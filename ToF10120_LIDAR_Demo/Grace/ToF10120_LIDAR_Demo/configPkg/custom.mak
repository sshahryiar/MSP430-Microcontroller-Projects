## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,430 linker.cmd package/cfg/ToF10120_LIDAR_Demo_p430.o430

linker.cmd: package/cfg/ToF10120_LIDAR_Demo_p430.xdl
	$(SED) 's"^\"\(package/cfg/ToF10120_LIDAR_Demo_p430cfg.cmd\)\"$""\"E:/Current Works/ToF10120_LIDAR_Demo/Grace/ToF10120_LIDAR_Demo/configPkg/\1\""' package/cfg/ToF10120_LIDAR_Demo_p430.xdl > $@
