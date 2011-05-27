# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2011 all rights reserved
#


# helper routine to initialize the extension module
def initializeExtension():
    # access the extension
    from . import pyrepg
    # get hold of the standard compliant exception hierarchy
    from ..db import exceptions
    # register the exception hierarchy with the module so that the exceptions it raises are
    # subclasses of the ones defined in pyre.db
    pyrepg.registerExceptions(exceptions)
    # and return the module
    return pyrepg


# declaration
class Connection:
    """
    This is a sample documentation string for class Connection
    """


    # class public data
    pyrepg = initializeExtension() # the handle to the postgres extension module


    # interface
    def close(self):
        """
        Close the connection to the database

        Closing a connection makes it unsuitable for any further database access. This applies
        to all objects that may retain a reference to the connection being closed. Any
        uncommitted changes will be lost
        """
        return self.pyrepg.disconnect(self.connection)


    def execute(self, command):
        """
        Execute command and return the result
        """
        return self.pyrepg.execute(self.connection, command)


    # meta methods
    def __init__(self, *, # keyword arguments only from here on
                 # only a subset of the connection speficiations parameters are supported 
                 database, # the name of the database to access
                 user=None, password=None, # authentication
                 application=None,
                 **kwds):

        # chain to the ancestors
        super().__init__(**kwds)

        # build the connection specification string
        spec = [
            ['dbname', database]
            ]
        if user is not None: spec.append(['user', user])
        if password is not None: spec.append(('password', password))
        # if application is not None: spec.append(('application_name', application))
        spec = ' '.join([ '='.join(entry) for entry in spec ])
        
        # establish a connection
        self.connection = self.pyrepg.connect(spec)

        # all done
        return


# end of file 
