# invoke SourceDir generated makefile for ToF10120_LIDAR_Demo.p430
ToF10120_LIDAR_Demo.p430: .libraries,ToF10120_LIDAR_Demo.p430
.libraries,ToF10120_LIDAR_Demo.p430: package/cfg/ToF10120_LIDAR_Demo_p430.xdl
	$(MAKE) -f E:\Current Works\ToF10120_LIDAR_Demo\Grace\ToF10120_LIDAR_Demo/makefile.libs

clean::
	$(MAKE) -f E:\Current Works\ToF10120_LIDAR_Demo\Grace\ToF10120_LIDAR_Demo/makefile.libs clean

