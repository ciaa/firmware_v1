For the PEAK PCAN driver read ../libs/pcan/README.txt .

For the Sontheim driver download the MT-API from the download center at
    http://www.s-i-e.de/
and run the contained installation program. Install to
    C:\Programme\Sontheim\MT_Api .
If it's already installed in a different path, then adapt
    can_server_sontheim_mt.vcproj .

For the Vector driver download
    http://www.vector-worldwide.com/downloads/drivers/canlib43.zip
and run the contained setup program. Install to
    c:\Programme\CANLIB .
If it's already installed in a different path, then adapt
    can_server_vector.vcproj .

For the Vector XL driver download
    http://www.vector-worldwide.com/downloads/drivers/xl_lib67.zip
and run the contained setup program. Install to
    C:\Programme\XL Driver Library .
If it's already installed in a different path, then adapt
    can_server_vector_xl.vcproj .

For the LAWICEL driver download
    http://www.canusb.com/drivers/canusbdrv018.zip
and
    http://www.canusb.com/cdm/CDM_2.04.06_CANUSB.zip

Follow the driver installation guide
    http://www.canusb.com/documents/canusb_drinst_vcp.pdf
When reading, particularly find out if you have to temporarily
disconnect the PC from the Internet during installation.
Eventually install to
    C:\Programme\LAWICEL\CANUSB .
If it's already installed in a different path, then adapt
    can_server_lawicel.vcproj .

Note: Only if you chose ``Release'' rather than ``Debug'' as MSVC++
solution configuration, then the resulting can server executable will
be copied to ..\..\bin\win32\ during the build.
