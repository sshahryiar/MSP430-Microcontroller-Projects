#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#

unexport MAKEFILE_LIST
MK_NOGENDEPS := $(filter clean,$(MAKECMDGOALS))
override PKGDIR = configPkg
XDCINCS = -I. -I$(strip $(subst ;, -I,$(subst $(space),\$(space),$(XPKGPATH))))
XDCCFGDIR = package/cfg/

#
# The following dependencies ensure package.mak is rebuilt
# in the event that some included BOM script changes.
#
ifneq (clean,$(MAKECMDGOALS))
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/utils.js:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/utils.js
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/xdc.tci:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/xdc.tci
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/template.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/template.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/om2.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/om2.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/xmlgen.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/xmlgen.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/xmlgen2.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/xmlgen2.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/Warnings.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/Warnings.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/IPackage.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/IPackage.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/package.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/package.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/services/global/Clock.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/services/global/Clock.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/services/global/Trace.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/services/global/Trace.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/bld.js:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/bld.js
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/BuildEnvironment.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/BuildEnvironment.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/PackageContents.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/PackageContents.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/_gen.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/_gen.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Library.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Library.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Executable.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Executable.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Repository.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Repository.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Configuration.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Configuration.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Script.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Script.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Manifest.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Manifest.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Utils.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/Utils.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/ITarget.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/ITarget.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/ITarget2.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/ITarget2.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/ITarget3.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/ITarget3.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/ITargetFilter.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/ITargetFilter.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/package.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/bld/package.xs
package.mak: config.bld
D:/ti/grace/xdctools_3_25_04_88/packages/ti/targets/ITarget.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/ti/targets/ITarget.xs
D:/ti/grace/xdctools_3_25_04_88/packages/ti/targets/C28_large.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/ti/targets/C28_large.xs
D:/ti/grace/xdctools_3_25_04_88/packages/ti/targets/C28_float.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/ti/targets/C28_float.xs
D:/ti/grace/xdctools_3_25_04_88/packages/ti/targets/package.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/ti/targets/package.xs
D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/MSP430.xs:
package.mak: D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/MSP430.xs
D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/MSP430X.xs:
package.mak: D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/MSP430X.xs
D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/MSP430X_small.xs:
package.mak: D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/MSP430X_small.xs
D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/Generic.xs:
package.mak: D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/Generic.xs
D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/package.xs:
package.mak: D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/package.xs
package.mak: package.bld
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/compiler.opt.xdt:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/compiler.opt.xdt
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/services/io/File.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/services/io/File.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/services/io/package.xs:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/services/io/package.xs
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/compiler.defs.xdt:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/compiler.defs.xdt
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/custom.mak.exe.xdt:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/custom.mak.exe.xdt
D:/ti/grace/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/package.xs.xdt:
package.mak: D:/ti/grace/xdctools_3_25_04_88/packages/xdc/tools/configuro/template/package.xs.xdt
endif

ti.targets.msp430.Generic.rootDir ?= .
ti.targets.msp430.packageBase ?= D:/ti/grace/grace_2_20_02_32/packages/ti/targets/msp430/
.PRECIOUS: $(XDCCFGDIR)/%.o430
.PHONY: all,430 .dlls,430 .executables,430 test,430
all,430: .executables,430
.executables,430: .libraries,430
.executables,430: .dlls,430
.dlls,430: .libraries,430
.libraries,430: .interfaces
	@$(RM) $@
	@$(TOUCH) "$@"

.help::
	@$(ECHO) xdc test,430
	@$(ECHO) xdc .executables,430
	@$(ECHO) xdc .libraries,430
	@$(ECHO) xdc .dlls,430


all: .executables 
.executables: .libraries .dlls
.libraries: .interfaces

PKGCFGS := $(wildcard package.xs) package/build.cfg
.interfaces: package/package.xdc.inc package/package.defs.h package.xdc $(PKGCFGS)

-include package/package.xdc.dep
package/%.xdc.inc package/%_configPkg.c package/%.defs.h: %.xdc $(PKGCFGS)
	@$(MSG) generating interfaces for package configPkg" (because $@ is older than $(firstword $?))" ...
	$(XSRUN) -f xdc/services/intern/cmd/build.xs $(MK_IDLOPTS) -m package/package.xdc.dep -i package/package.xdc.inc package.xdc

.dlls,430 .dlls: ToF10120_LIDAR_Demo.p430

-include package/cfg/ToF10120_LIDAR_Demo_p430.mak
-include package/cfg/ToF10120_LIDAR_Demo_p430.cfg.mak
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/ToF10120_LIDAR_Demo_p430.dep
endif
ToF10120_LIDAR_Demo.p430: package/cfg/ToF10120_LIDAR_Demo_p430.xdl
	@


ifeq (,$(wildcard .libraries,430))
ToF10120_LIDAR_Demo.p430 package/cfg/ToF10120_LIDAR_Demo_p430.c: .libraries,430
endif

package/cfg/ToF10120_LIDAR_Demo_p430.c package/cfg/ToF10120_LIDAR_Demo_p430.h package/cfg/ToF10120_LIDAR_Demo_p430.xdl: override _PROG_NAME := ToF10120_LIDAR_Demo.x430
package/cfg/ToF10120_LIDAR_Demo_p430.c package/cfg/ToF10120_LIDAR_Demo_p430.xdl: override _PROG_XSOPTS = -Dxdc.cfg.SourceDir.outputDir='E%3A%5CCurrent%20Works%5CToF10120_LIDAR_Demo%5CGrace%5CToF10120_LIDAR_Demo'
package/cfg/ToF10120_LIDAR_Demo_p430.c: package/cfg/ToF10120_LIDAR_Demo_p430.cfg

clean:: clean,430
	-$(RM) package/cfg/ToF10120_LIDAR_Demo_p430.cfg
	-$(RM) package/cfg/ToF10120_LIDAR_Demo_p430.dep
	-$(RM) package/cfg/ToF10120_LIDAR_Demo_p430.c
	-$(RM) package/cfg/ToF10120_LIDAR_Demo_p430.xdc.inc

clean,430::
	-$(RM) ToF10120_LIDAR_Demo.p430
.executables,430 .executables: ToF10120_LIDAR_Demo.x430

ToF10120_LIDAR_Demo.x430: |ToF10120_LIDAR_Demo.p430

-include package/cfg/ToF10120_LIDAR_Demo.x430.mak
ToF10120_LIDAR_Demo.x430: package/cfg/ToF10120_LIDAR_Demo_p430.o430 
	$(RM) $@
	@$(MSG) lnk430 $@ ...
	:

ToF10120_LIDAR_Demo.test test,430 test: ToF10120_LIDAR_Demo.x430.test

ToF10120_LIDAR_Demo.x430.test:: ToF10120_LIDAR_Demo.x430
ifeq (,$(_TESTLEVEL))
	@$(MAKE) -R -r --no-print-directory -f $(XDCROOT)/packages/xdc/bld/xdc.mak _TESTLEVEL=1 ToF10120_LIDAR_Demo.x430.test
else
	@$(MSG) running $<  ...
	$(call EXEC.ToF10120_LIDAR_Demo.x430, ) 
endif

clean,430::
	-$(RM) .tmp,ToF10120_LIDAR_Demo.x430,0,*


clean:: clean,430

clean,430::
	-$(RM) ToF10120_LIDAR_Demo.x430
%,copy:
	@$(if $<,,$(MSG) don\'t know how to build $*; exit 1)
	@$(MSG) cp $< $@
	$(RM) $@
	$(CP) $< $@
ToF10120_LIDAR_Demo_p430.o430,copy : package/cfg/ToF10120_LIDAR_Demo_p430.o430

$(XDCCFGDIR)%.c $(XDCCFGDIR)%.h $(XDCCFGDIR)%.xdl: $(XDCCFGDIR)%.cfg .interfaces $(XDCROOT)/packages/xdc/cfg/Main.xs
	@$(MSG) "configuring $(_PROG_NAME) from $< ..."
	$(CONFIG) $(_PROG_XSOPTS) xdc.cfg $(_PROG_NAME) $(XDCCFGDIR)$*.cfg $(XDCCFGDIR)$*

.PHONY: release,configPkg
configPkg.tar: package/package.bld.xml
configPkg.tar: package/build.cfg
configPkg.tar: package/package.xdc.inc
ifeq (,$(MK_NOGENDEPS))
-include package/rel/configPkg.tar.dep
endif
package/rel/configPkg/configPkg/package/package.rel.xml: .force
	@$(MSG) generating external release references $@ ...
	$(XS) $(JSENV) -f $(XDCROOT)/packages/xdc/bld/rel.js $(MK_RELOPTS) . $@

configPkg.tar: package/rel/configPkg.xdc.inc package/rel/configPkg/configPkg/package/package.rel.xml
	@$(MSG) making release file $@ "(because of $(firstword $?))" ...
	-$(RM) $@
	$(call MKRELTAR,package/rel/configPkg.xdc.inc,package/rel/configPkg.tar.dep)


release release,configPkg: all configPkg.tar
clean:: .clean
	-$(RM) configPkg.tar
	-$(RM) package/rel/configPkg.xdc.inc
	-$(RM) package/rel/configPkg.tar.dep

clean:: .clean
	-$(RM) .libraries .libraries,*
clean:: 
	-$(RM) .dlls .dlls,*
#
# The following clean rule removes user specified
# generated files or directories.
#

ifneq (clean,$(MAKECMDGOALS))
ifeq (,$(wildcard package))
    $(shell $(MKDIR) package)
endif
ifeq (,$(wildcard package/cfg))
    $(shell $(MKDIR) package/cfg)
endif
ifeq (,$(wildcard package/lib))
    $(shell $(MKDIR) package/lib)
endif
ifeq (,$(wildcard package/rel))
    $(shell $(MKDIR) package/rel)
endif
ifeq (,$(wildcard package/internal))
    $(shell $(MKDIR) package/internal)
endif
endif
clean::
	-$(RMDIR) package

include custom.mak
