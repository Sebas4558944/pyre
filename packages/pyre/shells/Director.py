# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2022 all rights reserved
#


# externals
import weakref
# framework access
import pyre


# declaration
class Director(pyre.actor):
    """
    The metaclass of applications

    {Director} takes care of all the tasks necessary to register an application family with the
    framework
    """


    # meta methods
    def __init__(self, name, bases, attributes, namespace=None, **kwds):
        """
        Initialization of application class records
        """
        # chain up
        super().__init__(name, bases, attributes, **kwds)

        # if i don't have a namespace
        if not namespace:
            # get my package
            package = self.pyre_package()
            # and if it exists
            if package:
                # use its name as my namespace
                namespace = package.name
        # attach it
        self.pyre_namespace = namespace

        # all done
        return


    def __call__(self, globalAliases=True, **kwds):
        """
        Build an application instance
        """
        # just chain up; my job is to reset the default value of {globalAliases}
        return super().__call__(globalAliases=globalAliases, **kwds)


# end of file
