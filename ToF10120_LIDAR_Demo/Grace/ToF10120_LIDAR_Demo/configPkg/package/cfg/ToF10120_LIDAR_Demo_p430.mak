#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.msp430.Generic{1,0,0
#
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/ToF10120_LIDAR_Demo_p430.o430.dep
package/cfg/ToF10120_LIDAR_Demo_p430.o430.dep: ;
endif

package/cfg/ToF10120_LIDAR_Demo_p430.o430: | .interfaces
package/cfg/ToF10120_LIDAR_Demo_p430.o430: package/cfg/ToF10120_LIDAR_Demo_p430.c package/cfg/ToF10120_LIDAR_Demo_p430.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cl430 $@ ...
	:
	-@$(FIXDEP) $@.dep $@.dep
	

clean,430 ::
	-$(RM) package/cfg/ToF10120_LIDAR_Demo_p430.o430

ToF10120_LIDAR_Demo.p430: package/cfg/ToF10120_LIDAR_Demo_p430.o430 package/cfg/ToF10120_LIDAR_Demo_p430.mak

clean::
	-$(RM) package/cfg/ToF10120_LIDAR_Demo_p430.mak
