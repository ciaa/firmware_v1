About this commercial_BIOS directory
------------------------------------

Note that when working on Windows or Linux no commercial BIOS files are needed.

If you're compiling for an embedded target, you may already have the according 
BIOS available, so you would just need to set that path in your IsoAgLib 
configuration file (conf_<projectname>).

If you'd like to use the default, then you just need to copy the commercial 
BIOS into a bios_<platform> directory (e.g. bios_esx) or you can still
checkout the subrepositories (that were formerly used) to there.
In this case please get in contact with OSB AG to get access to these.

Valid directory-names for the available HALs are:
 - bios_c2c
 - bios_Dj1
 - bios_esx
 - bios_esxu
 - bios_p1mc
