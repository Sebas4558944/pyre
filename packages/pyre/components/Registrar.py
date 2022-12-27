# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2023 all rights reserved
#


# externals
import weakref
import collections


# class declaration
class Registrar:
    """
    The manager of protocols, component classes and their instances

    All user defined protocols and components are registered with {Registrar} as they
    are encountered by the framework. Clients can discover the protocol and component classes
    that are registered, the set of instances of any component, as well as the components that
    implement a particular protocol.

    The two base classes {pyre.components.Protocol} and {pyre.components.Component}, as well
    as the protocol specifications autogenerated by {Role}, are declared with the special
    attribute {internal} set to {True} and they are not registered.
    """


    # public data
    protocols = None # the set of known protocols
    components = None # the map of component classes to their instances
    implementers = None # a map of protocols to component classes that implement them


    # interface
    def registerNamingServer(self, server):
        """
        Register {server} as a naming service
        """
        # add it to my pile
        self.nameGenerators.add(server)
        # all done
        return self


    def registerProtocolClass(self, protocol):
        """
        Register the {protocol} class record
        """
        # add to the pile
        self.protocols.add(protocol)
        # notify all observers
        for observer in self.protocolObservers:
            # by invoking the hook
            observer.pyre_newProtocolRegistration(protocol=protocol)
        # and hand the protocol back to the caller
        return protocol


    def registerComponentClass(self, component):
        """
        Register the {component} class record
        """
        # prime the map of components to their instances
        self.components[component] = weakref.WeakSet()
        # update the map of protocols it implements
        for protocol in self.findRegisteredProtocols(component):
            # by registering this component as an implementor
            self.implementers[protocol].add(component)
        # notify all observers
        for observer in self.componentObservers:
            # by invoking the hook
            observer.pyre_newComponentRegistration(component=component)
        # and hand the component back to the caller
        return component


    def registerComponentInstance(self, instance):
        """
        Register this component instance
        """
        # add this instance to the set of instances of its class
        self.components[type(instance)].add(instance)
        # notify all observers
        for observer in self.instanceObservers:
            # by invoking the hook
            observer.pyre_newInstanceRegistration(instance=instance)
        # and return it
        return instance


    def nameInstance(self, componentClass):
        """
        Attempt to generate a name for an instance of {componentClass}
        """
        # go through the registered name generators
        for namegen in self.nameGenerators:
            # ask each one for a name
            name = namegen.nameInstance(componentClass=componentClass)
            # the first one that returns something non-trivial
            if name is not None:
                # gets to decide the name of the component instance
                return name
        # out of ideas
        return None


    def observeProtocols(self, observer):
        """
        Add {observer} to the set of entities interested in the registration of new protocols
        """
        # add {observer} to the pile
        self.protocolObservers.add(observer)
        # all done
        return self


    def observeComponents(self, observer):
        """
        Add {observer} to the set of entities interested in the registration of new components
        """
        # add {observer} to the pile
        self.componentObservers.add(observer)
        # all done
        return self


    def observeInstances(self, observer):
        """
        Add {observer} to the set of entities interested in the registration of new instances
        """
        # add {observer} to the pile
        self.instanceObservers.add(observer)
        # all done
        return self


    def publicProtocols(self):
        """
        Generate a topologically sorted sequence of registered public protocols
        """
        # the previously visited protocols
        visited = set()
        # go through all the public registered protocols
        for protocol in filter(lambda x: x.pyre_key, self.protocols):
            # visit them and sort them
            yield from self.depthFirst(configurable=protocol, visited=visited)
        # all done
        return


    def publicImplementers(self, protocol):
        """
        Generate a sequence of public components that implement the given {protocol}
        """
        # go through all the implementers and filter out the ones that don't have a public key
        yield from (
            # grab components
            component
            # by picking from the implementers of the protocol
            for component in self.implementers[protocol]
            # the ones with a public key only
            if component.pyre_isPublicClass()
            )
        # all done
        return


    # implementation details
    def findRegisteredProtocols(self, component):
        """
        Build a sequence of the registered protocols that are implemented by this component
        """
        # get the interface implementation specification
        implements = component.pyre_implements
        # if there aren't any, bail out
        if implements is None: return
        # otherwise, loop over the protocol mro
        for protocol in implements.__mro__:
            # ignore the trivial ones by checking with the registry
            if protocol in self.protocols:
                # before sending it out
                yield protocol
        # all done
        return


    def retrieveComponentByName(self, componentClass, name):
        """
        Look through the registered instances of {componentClass} for one with the given {name}
        """
        # go through the pile
        for instance in self.components[componentClass]:
            # return the instance whose the name matches the given one
            if instance.pyre_name == name:  return instance
        # otherwise, no match
        return None


    # meta-methods
    def __init__(self, **kwds):
        # chain up
        super().__init__(**kwds)
        # map: components -> their instances
        self.components = {}
        # the known interfaces
        self.protocols = set()
        # map: protocols -> components that implement them
        self.implementers = collections.defaultdict(set)

        # registered services
        self.nameGenerators = weakref.WeakSet()

        # listeners
        self.protocolObservers = weakref.WeakSet()
        self.componentObservers = weakref.WeakSet()
        self.instanceObservers = weakref.WeakSet()

        # all done
        return


    # implementation details
    def depthFirst(self, configurable, visited):
        """
        Workhorse for the protocol traversal in topologically sorted order
        """
        # if we have visited this {configurable} before
        if configurable in visited:
            # we are done
            return
        # otherwise, grab the public ancestors
        for base in configurable.pyre_public():
            # skip the one we are working with
            if base is configurable: continue
            # visit the rest
            yield from self.depthFirst(configurable=base, visited=visited)
        # add this one to the pile
        visited.add(configurable)
        # and send it to the caller
        yield configurable
        # all done
        return


# end of file
