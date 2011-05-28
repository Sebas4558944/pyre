# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2011 all rights reserved
#


import os
import re
import itertools


class Executive:
    """
    The top level framework object

    This class maintains a suite of managers that are responsible for the various mechanisms
    and policies that enable pyre applications. The Executive orchestrates their interactions
    and provides the top level interface of the framework.

    The actual executive is an instance of the class Pyre, also in this package. Pyre is a
    singleton that is accessible as pyre.executive. For more details, see Pyre.py and
    __init__.py in this package.
    """


    # exceptions
    from .exceptions import FrameworkError, BadResourceLocatorError
    from .exceptions import ComponentNotFoundError, SymbolNotFoundError
    from ..config.exceptions import DecodingError


    # public data
    configpath = ()
    # managers
    binder = None
    codex = None
    configurator = None
    fileserver = None
    registrar = None
    timekeeper = None
    resolvers = None

    # book keeping
    packages = None
    errors = None

    # constants
    defaultLocations = ("/pyre/system", "/pyre/user", "/local")
    path = tuple('vfs:' + location for location in defaultLocations)

    # priority levels for the various configuration sources
    DEFAULT_CONFIGURATION = -1 # defaults from the component declarations
    BOOT_CONFIGURATION = 0 # configuration from the standard pyre boot files
    PACKAGE_CONFIGURATION = 5 # configuration from package files
    USER_CONFIGURATION = 10 # configurations supplied by the end user
    EXPLICIT_CONFIGURATION = 15 # programmatic overrides


    # external interface
    def loadConfiguration(self, uri, priority=USER_CONFIGURATION, locator=None):
        """
        Load configuration settings from {uri} and insert them in the configuration database
        with the given {priority}.
        """
        # decode the uri
        scheme, authority, address, query, fragment = self.parseURI(uri)
        # get the fileserver to  deduce the encoding and produce the input stream
        encoding, source = self.fileserver.open(scheme, address=address)
        # instantiate the requested reader
        reader = self.codex.newCodec(encoding)
        # extract the configuration setting from the source
        configuration = reader.decode(source=source, locator=locator)
        # update the evaluation model
        errors = self.configurator.configure(configuration=configuration, priority=priority)
        # add any errors to the pile
        self.errors.extend(errors)
        # all done
        return self


    # other facilities
    def newTimer(self, **kwds):
        """
        Build and return a timer
        """
        # let the timer registry do its thing
        return self.timekeeper.timer(**kwds)


    def registerNamespaceResolver(self, resolver, namespace):
        """
        Add {resolver} to the table of entities that get notified when an unqualified component
        resolution request is made

        For example, {pyre.shells.Director} registers an application class as a resolver of
        requests within the namespace specified by the application family. This way,
        unqualified requests for facility bindings can be resolved in an application specific
        manner. Look at {Executive.retrieveComponentDescriptor} for more details
        """
        # register the {resolver} under the key {namespace}
        self.resolvers[namespace] = resolver
        # and return
        return


    # support for the various internal requests
    def configurePackage(self, package):
        """
        Locate and load the configuration files for the package to which {component} belongs

        If the package to which {component} belongs can be deduced from its family name, this
        method will locate and load the package configuration files. These files are meant to
        allow site managers and end users to override the class wide defaults for the traits of
        the components in the package.
        
        This behavior is triggered by the first encountered component from each package, and it

        is done only once.
        """
        # if none were provided, there is no file-based configuration
        if not package: return package
        # also, bail out if this package has been configured previously
        if package in self.packages: return
        # we have a package name
        # print("Executive.configurePackage: configuring package {!r}".format(package))
        # form all possible filenames for the configuration files
        scope = itertools.product(reversed(self.configpath), [package], self.codex.getEncodings())
        # attempt to load the configuration settings
        for path, filename, extension in scope:
            # construct the actual filename
            source = self.fileserver.splice(path, filename, extension)
            # and try to load the configuration
            try:
                self.loadConfiguration(uri=source, priority=self.PACKAGE_CONFIGURATION)
            except self.fileserver.NotFoundError as error:
                continue
            # print("Executive.configurePackage: loaded {!r}".format(source))
        # in any case, this is the best that can be done for this package
        # update the set of known packages
        self.packages.add(package)
        # print("Executive.configurePackage: done; packages={}".format(self.packages))
        # all done
        return package


    def retrieveComponentDescriptor(self, uri, context=None, locator=None):
        """
        Interpret {uri} as a component descriptor and attempt to resolve it

        {uri} encodes the descriptor using the URI specification 
            scheme://authority/address#namme
        where 
            {scheme}: the resolution mechanism
            {authority}: the process that will perform the resolution
            {address}: the location of the component descriptor
            {name}: the optional name to use when instantiating the retrieved descriptor

        Currently, there is support for two classes of schemes:

        The "import" scheme requires that the component descriptor is accessible on the python
        path. The corresponding codec interprets {address} as two parts: {package}.{symbol},
        with {symbol} being the trailing part of {address} after the last '.'. The codec then
        uses the interpreter to import the symbol {symbol} using {address} to access the
        containing module. For example, the {uri}

            import:package.subpackage.module.myFactory

        is treated as if the following statement had been issued to the interpreter

            from package.subpackage.module import myFactory

        See below for the requirements myFactory must satisfy

        Any other scheme specification is interpreted as a request for a file based component
        factory. The {address} is again split into two parts: {path}/{symbol}, where {symbol}
        is the trailing part after the last '/' separator. The codec assumes that {path} is a
        valid path in the physical or logical filesystems managed by the executive.fileserver,
        and that it contains executable python code that provides the definition of the
        required symbol.  For example, the {uri}

            vfs:/local/sample.odb/myFactory

        expects that the fileserver can resolve the address local/sample.odb into a valid file
        within the virtual filesystem that forms the application namespace.

        The symbol referenced by the {symbol} fragment must be a callable that can produce
        component class records when called. For example

            def myFactory():
                import pyre
                class mine(pyre.component): pass
                return mine

        would be valid contents for an accessible module or an odb file.
        """
        # make sure {context} is iterable
        context = context if context is not None else ()
        # attempt to parse the {uri}
        try:
            scheme, _, address, _, name = self.parseURI(uri)
        except self.BadResourceLocatorError as error:
            raise self.ComponentNotFoundError(uri=uri) from error

        # if {uri} contains a scheme, use it; otherwise, try the default ones
        schemes = [scheme] if scheme else ["vfs", "import"]

        # iterate over the candidate schemes
        for scheme in schemes:
            # split the address into a package and a symbol
            package, symbol = self._parseDescriptorAddress(scheme=scheme, address=address)
            # if successful, use the given package; otherwise, build one from the auxiliary info
            packages = [package] if package else self._buildPackageContext(scheme, context)
            # iterate over the candidate package names
            for package in packages:
                # print("scheme: {!r}, package: {!r}, symbol: {!r}".format(scheme,package,symbol))
                # try to locate the shelf associated with the ({scheme}, {address}) pair
                try:
                    shelf = self._loadShelf(scheme=scheme, address=package, locator=locator)
                except:
                    continue
                # retrieve the descriptor
                descriptor = shelf.retrieveSymbol(symbol=symbol)
                # if there was no name specified, return the descriptor
                if name is None: return descriptor
                # otherwise, build a component instance and return it
                return descriptor(name=name)

        # otherwise
        raise self.ComponentNotFoundError(uri=uri)

        # make sure {context} is iterable
        context = context if context is not None else ()
        # attempt to parse the {uri}
        try:
            # pull out only, the currently supported parts
            scheme, _, address, _, name = self.parseURI(uri)
        # if parsing failed, it is a badly formed request
        except self.BadResourceLocatorError as error:
            # just give up
            raise self.ComponentNotFoundError(uri=uri) from error

        # if {uri} contains a scheme, use it; otherwise, try all the options
        schemes = [scheme] if scheme else ["vfs", "import"]

        # iterate over the encoding possibilities
        for scheme in schemes:
            # build a codec for this candidate scheme
            codec = self.codex.newCodec(encoding=scheme, client=self)
            # locate a candidate shelf
            codec.locateSymbol(address=address, context=context, locator=locator)

        # otherwise
        raise self.ComponentNotFoundError(uri=uri)


    def locateComponentDescriptor(self, component, locations):
        """
        Attempt to retrieve {component} from the first shelf within {locations}
        """
        # print("Executive.locateComponentDescriptor: looking for {!r}".format(component))
        # iterate over the given locations
        for location in locations:
            # print("  looking in {!r}".format(location))
            # ask the file server for the matching folder
            try:
                folder = self.fileserver[location]
            # if not there, move on...
            except self.fileserver.NotFoundError:
                # print("    locations does not exist")
                continue
            # now, iterate over the contents of the folder
            for entry in folder.contents:
                # print("    opening {!r}".format(entry))
                # form the name of the item
                address = folder.join(location, entry)
                # try to load the associated shelf
                try:
                    shelf = self._loadShelf(scheme="vfs", address=address, locator=None)
                except self.FrameworkError:
                    continue
                # try to look up the symbol
                try:
                    descriptor = shelf.retrieveSymbol(symbol=component)
                # if not there, try the next entry in the folder
                except self.FrameworkError:
                    continue
                # and return it
                return descriptor
        # couldn't find the spell
        raise self.SymbolNotFoundError(symbol=component)
            

    # registration of configurables
    def registerComponentClass(self, component):
        """
        Register the {component} class record
        """
        # register the component
        self.registrar.registerComponentClass(component)
        # invoke the registration hook
        component.pyre_registerClass(executive=self)
        # load the package configuration; must do this before configuring the class
        self.configurePackage(package=component.pyre_getPackageName())
        # populate the class defaults with the configuration information
        errors = self.configurator.configureComponentClass(self.registrar, component)
        # add any errors encountered to the pile
        self.errors.extend(errors)
        # invoke the configuration hook
        component.pyre_configureClass(executive=self)
        # bind the component
        self.binder.bindComponentClass(component)
        # invoke the initialization hook
        component.pyre_initializeClass(executive=self)
        # and hand back the class record
        return component


    def registerComponentInstance(self, component):
        """
        Register the {component} instance
        """
        # register the component instance
        self.registrar.registerComponentInstance(component)
        # invoke the registration hook
        component.pyre_register(executive=self)
        # configure it
        errors = self.configurator.configureComponentInstance(self.registrar, component)
        # add any errors encountered to the pile
        self.errors.extend(errors)
        # invoke the configuration hook
        component.pyre_configure(executive=self)
        # bind the component
        self.binder.bindComponentInstance(component)
        # invoke the binding hook
        component.pyre_bind(executive=self)
        # invoke the initialization hook
        component.pyre_initialize(executive=self)
        # and hand the instance back to the caller
        return component


    def registerInterfaceClass(self, interface):
        """
        Register the {interface} class record
        """
        # register the interface
        self.registrar.registerInterfaceClass(interface)
        # invoke the registration hook
        interface.pyre_registerClass(executive=self)
        # and hand back the class record
        return interface


    # access to the shelf registry
    def registerShelf(self, shelf, source):
        """
        Record the {source} that corresponds to the given {shelf}
        """
        # add {source} to the dictionary with the loaded shelves
        self.shelves[source] = shelf
        # and return
        return self


    def loadShelf(self, uri, locator=None):
        """
        Load the contents of the shelf pointed to by {uri}

        {uri} encodes the descriptor using the URI specification 
            scheme://authority/address
        where
             scheme: one of import, file, vfs
             authority: currently not used; you may leave blank
             address: a scheme dependent specification of the location of the shelf
        """
        # parse the {uri}
        scheme, authority, address, query, symbol = self.parseURI(uri)
        # and retrieve the shelf
        return self._loadShelf(scheme=scheme, address=address, locator=locator)


    # utilities
    def parseURI(self, uri):
        """
        Extract the scheme, address and fragment from {uri}.
        """
        # run uri through the recognizer
        match = self._uriRecognizer.match(uri)
        # if it fails to match, it must be malformed (or my regex is bad...)
        if match is None:
            raise self.BadResourceLocatorError(uri=uri, reason="unrecognizable")
        # extract the scheme
        scheme = match.group("scheme")
        # extract the authority
        authority = match.group("authority")
        # extract the address
        address = match.group("address")
        # extract the query
        query = match.group("query")
        # extract the fragment
        fragment = match.group("fragment")
        # and return the triplet
        return scheme, authority, address, query, fragment


    def normalizeURI(self, scheme=None, authority=None, address=None, query=None, symbol=None):
        """
        Construct a uri in normal form
        """
        # initialize the fragment container
        uri = []
        # handle the scheme
        if scheme is not None:
            uri.append(scheme + ":")
        # handle the authority
        if authority is not None:
            uri.append("//")
            uri.append(authority)
        # handle the address
        if address is not None:
            uri.append(address)
        # handle the query
        if query is not None:
            uri.append("?")
            uri.append(query)
        # handle the symbol
        if symbol is not None:
            uri.append("#")
            uri.append(symbol)
        # assemble and return
        return "".join(uri)


    # meta methods
    def __init__(self, managers, **kwds):
        super().__init__(**kwds)

        # the timer manager
        self.timekeeper = managers.newTimerRegistrar()
        # build and start a timer
        self.timer = self.timekeeper.timer(name="pyre").start()

        # the manager of the component interdependencies
        self.binder = managers.newBinder()
        # my codec manager
        self.codex = managers.newCodecManager()
        # my configuration manager
        self.configurator = managers.newConfigurator(executive=self)
        # the manager of my virtual filesystem
        self.fileserver = managers.newFileServer()
        # the component registrar
        self.registrar = managers.newComponentRegistrar()
        # the map of namespaces to the entities that resolve name requests
        self.resolvers = {}

        # prime the configuration folder list
        self.configpath = list(self.path)

        # initialize the set of known packages
        self.packages = set()

        # initialize the list of errors encountered during configuration
        self.errors = []

        # initialize the set of known configuration sources
        self.shelves = {}

        # all done
        return


    # implementation details
    def _loadShelf(self, scheme, address, locator):
        """
        Load the contents of the shelf pointed to by the given URI fragments
        """
        # adjust the scheme, if necessary
        scheme = scheme.strip().lower() if scheme else 'file'
        # construct the normal form for the filename part
        source = self.normalizeURI(scheme=scheme, address=address)
        # build a locator, if necessary
        if locator is None:
            from ..tracking import newFileLocator
            locator = newFileLocator(source=source)
        # check whether we have processed this source before
        try:
            # get the shelf
            return self.shelves[source]
        # not there (yet)
        except KeyError:
            pass
        # if the scheme is "import", build a native codec
        if scheme == "import":
            # for the native codec, the source is the address fragment
            shelf = self.codex.newCodec(encoding="import").decode(source=address, locator=locator)
        else:
            # anything else is file based, for now
            # ask the file server to open the uri
            _, stream = self.fileserver.open(scheme=scheme, address=address)
            # build an odb codec
            codec = self.codex.newCodec(encoding="odb")
            # ask it for the shelf
            shelf = codec.decode(source=stream, locator=locator)

        # update the shelf index
        self.registerShelf(source=source, shelf=shelf)

        # and return the shelf
        return shelf


    def _parseDescriptorAddress(self, scheme, address):
        """
        Extract the package and symbol names from the given {address} specification
        """
        # build a codec for {scheme}
        codex = self.codex.newCodec(encoding=scheme)
        # ask it to extract the (package, symbol) tuple and return it
        return codex.parseAddress(address)


    def _buildPackageContext(self, scheme, context):
        """
        Use the given {scheme} to build possible package names out of {context}. This method is
        used by {retrieveComponentDescriptor} to create candidates for resolving unqualified
        symbols
        """
        # i don't see a way to handle both {vfs} and {import} symmetrically at this point, so
        # if the {scheme} is {import}
        if scheme == 'import':
            # get the {scheme} separator
            separator = self.codex.newCodec(encoding=scheme).separator
            # build an iteration over the possible sublists of context
            for marker in reversed(range(1, len(context)+1)):
                # form the candidate package name and yield it
                yield separator.join(context[:marker])
            # all done for {import}
            return

        # the only other scheme currently supported is {vfs}
        if scheme == 'vfs':
            # get the {scheme} separator
            separator = self.codex.newCodec(encoding=scheme).separator
            # iterate over the directories in my path, highest priority first
            for prefix in reversed(self.defaultLocations):
                # build an iteration over the possible sublists of context
                for marker in reversed(range(1, len(context)+1)):
                    # form the candidate package name and yield it
                    yield separator.join([prefix] + context[:marker]) + '.py'
                
            # all done for {vfs}
            return

        # all done
        return
                

    # private data
    _uriRecognizer = re.compile(
        "".join(( # adapted from http://regexlib.com/Search.aspx?k=URL
                r"^(?=[^&])", # disallow '&' at the beginning of uri
                r"(?:(?P<scheme>[^:/?#]+):)?", # grab the scheme
                r"(?://(?P<authority>[^/?#]*))?", # grab the authority
                r"(?P<address>[^?#]*)", # grab the address, typically a path
                r"(?:\?(?P<query>[^#]*))?", # grab the query, i.e. the ?key=value&... chunks
                r"(?:#(?P<fragment>.*))?"
                )))


# end of file 
