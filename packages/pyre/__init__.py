# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2012 all rights reserved
#


"""
pyre is a framework for building flexible applications

For more details, see http://pyre.caltech.edu.
For terms of use, see pyre.license()
"""


# imported symbols
import os

# convenience
def loadConfiguration(uri):
    """
    Open {uri} and attempt to load its contents into the configaration model
    """
    # build a locator for these settings
    here = tracking.here(level=1)
    # get the executive to do its thing
    return executive.loadConfiguration(uri=uri, locator=here)


# geography
def home():
    """
    Return the directory of the pyre package
    """
    return os.path.dirname(__file__)


def prefix():
    """
    Compute the pathname of the pyre installation
    """
    return os.path.abspath(os.path.join(home(), os.path.pardir, os.path.pardir))


def defaults():
    """
    Compute the pathname of the directory with the system defaults
    """
    return os.path.abspath(os.path.join(prefix(), 'defaults'))
    

# administrative
def copyright():
    """
    Return the pyre copyright note
    """
    return _pyre_copyright


def license():
    """
    Print the pyre license
    """
    print(_pyre_license)
    return


def version():
    """
    Return the pyre version
    """
    return _pyre_version


def credits():
    """
    Print the acknowledgments
    """
    print(_pyre_acknowledgments)
    return


# component introspection
def where(configurable, attribute=None):
    """
    Retrieve the location where the {attribute} of {configurable} got its value; if no
    {attribute} is specified, retrieve information about the {configurable} itself
    """
    # if no attribute name is given, return the locator of the configurable
    if attribute is None: return configurable.pyre_locator
    # retrieve the trait descriptor
    trait = configurable.pyre_trait(alias=attribute)
    # find the slot where the attribute is stored
    slot = trait.getSlot(configurable=configurable)
    # and return its locator
    return slot.locator


# settings
_pyre_version = (1, 0, 0)

_pyre_copyright = "pyre: Copyright (c) 1998-2012 Michael A.G. Aïvázis"

_pyre_license = """
    pyre 1.0
    Copyright (c) 1998-2012 Michael A.G. Aïvázis
    All Rights Reserved


    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.

    * Neither the name pyre nor the names of its contributors may be
      used to endorse or promote products derived from this software
      without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
    """

_pyre_acknowledgments = """
    pyre 1.0
    Copyright (c) 1998-2012 Michael A.G. Aïvázis
    All Rights Reserved


    This work has been funded in part by the Department of Energy, the National Science
    Foundation, the National Nuclear Safety Administration, and Orthologue, LLC.

    None of this would have been possible without Dan Meiron, Michael Ortiz, Mike Gurnis, Brent
    Fultz, and Mark Simons, all of whom took a risk by supporting pyre at various stages in its
    development.

    Much of the current design of pyre is due to ideas and constructive criticism by Leif
    Strand.

    Raúl Radovitzky, Brad Aagaard and Patrick Hung taught me a lot about the needs of massively
    parallel applications.
    
    Jiao Lin has built a very sophisticated web-based application by stretching pyre beyond its
    limits and contributing countless use cases that have had a significant impact on the
    current implementation.

    Mike McKerns, Tim Kelly, Brandon Keith are early adopters and have made significant
    contributions to the usability of the package. Most of the existing documentation and user
    support over the past ten years has been possible thanks to their remarkable efforts.
"""

# put the following start-up steps inside functions so we can have better control over their
# execution context and namespace pollution
def boot():
    """
    Perform all the initialization steps necessary to bootstrap the framework
    """
    # check the version of python
    import sys
    major, minor, micro, _, _ = sys.version_info
    if major < 3 or (major == 3 and minor < 2):
        from .framework.exceptions import PyreError
        raise PyreError(description="pyre needs python 3.2 or newer")

    # check whether the user has indicated we should skip booting
    try:
        import __main__
        if __main__.pyre_noboot: return None
    # if anything goes wrong
    except:
        # just ignore it and carry on
        pass

    # build, initialize and return the executive
    from . import framework
    return framework.executive().boot()


def debug():
    """
    Enable debugging of pyre modules.

    Modules that support debugging must provide a {debug} method and do as little as possible
    during their initialization. The fundamental constraints are provided by the python import
    algorithm that only give you one chance to import a module.

    This must be done very early, before pyre itself starts importing its packages. One way to
    request debugging is to create a variable {pyre_debug} in the __main__ module that contains
    a set of strings, each one of which is the name of a pyre module that you would like to
    debug.
    """
    # the set of packages to patch for debug support
    packages = set()
    # pull pyre_debug from the __main__ module
    import __main__
    try:
        packages |= set(__main__.pyre_debug)
    except:
        pass
    # iterate over the names, import the package and invoke its debug method
    for package in packages:
        module = __import__(package, fromlist=["debug"])
        module.debug()
    # all done
    return


# kickstart
# invoke the debug method in case the user asked for debugging support
debug()
# build the executive
executive = boot()

# if the framework booted properly
if executive:
    # for convenience
    from . import tracking
    # configurables and their support
    from .components.Actor import Actor as actor
    from .components.Role import Role as role
    from .components.Protocol import Protocol as protocol
    from .components.Component import Component as component
    # traits
    from .traits import properties
    from .traits.Behavior import Behavior as export
    from .traits.Behavior import Behavior as provides
    from .traits.Property import Property as property
    from .traits.Facility import Facility as facility
    from .traits.Map import Map as map
    from .traits.Catalog import Catalog as catalog
    # constraints
    from . import constraints
    # application
    from .shells import application
    # the base class of all pyre exceptions
    from .framework.exceptions import PyreError


# clean up the executive instance when the interpreter shuts down
import atexit
@atexit.register
def shutdown():
    """
    Attempt to hunt down and destroy all known references to the executive
    """
    # access the executive
    global executive
    # zero out the global reference
    executive = None
    # that should be enough
    return
    

# end of file 
