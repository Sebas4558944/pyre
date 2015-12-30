#!/usr/bin/env python.pyre
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2013 all rights reserved
#

# the framework
import pyre


# the app
class configure(pyre.application):
    """
    A sample configuration utility
    """

    postgres = pyre.externals.postgres()
    postgres.doc = "the postgres client installation"


    @pyre.export
    def main(self, *args, **kwds):
        """
        The main entry point
        """
        # get my channel
        info = self.info
        # show me
        info.line("{.pyre_name}:".format(self))
        info.line("  host: {.pyre_host}".format(self))
        info.line("  package manager: {.pyre_externals}".format(self))
        # flush
        info.log()

        # attempt to
        try:
            # get my postgres
            postgres = self.postgres
        # if something went wrong
        except self.ConfigurationError as error:
            # show me
            self.error.log(str(error))
            # and bail
            return 0

        # show me
        info.line("postgres:")
        info.line("  package: {}".format(postgres))
        # if i have one
        if postgres:
            # version info
            info.line("  version: {.version}".format(postgres))
            # locations
            info.line("  locations:")
            info.line("    prefix: {.prefix}".format(postgres))
            info.line("    incdir: {.incdir}".format(postgres))
            info.line("    libdir: {.libdir}".format(postgres))
            info.line("    client: {.psql}".format(postgres))
            # compile line
            info.line("  compile:")
            info.line("    defines: {}".format(', '.join(postgres.defines())))
            info.line("    headers: {}".format(' '.join(postgres.incdir)))
            # link line
            info.line("  link:")
            info.line("    paths: {}".format(' '.join(postgres.libdir)))
            info.line("    libraries: {}".format(' '.join(postgres.libraries())))

            # get the configuration errors
            errors = postgres.pyre_configurationErrors
            # if there were any
            if errors:
                # tell me
                info.line("  configuration errors that were auto-corrected:")
                # and show me
                for index, error in enumerate(errors):
                    info.line("      {}: {}".format(index+1, error))
        # flush
        info.log()

        # all done
        return 0


# main
if __name__ == "__main__":
    # get the journal
    import journal
    # activate the debug channel
    # journal.debug("app").activate()
    # make one
    app = configure(name='configure')
    # drive
    status = app.run()
    # all done
    raise SystemExit(status)


# end of file
