@echo off
echo Configuring GCC for go32
rem This batch file assumes a unix-type "sed" program

update config\xm-i386dos.h config.h
update config\xm-i386dos.h hconfig.h
update config\i386gas.h tm.h
update config\i386.md md
update config\i386.c aux-output.c

echo # Makefile generated by "configure.bat"> Makefile
echo all.dos: cccp cc1 cc1plus cc1obj xgcc>> Makefile

if exist config.sed del config.sed

echo "/^target=/ c\					">> config.sed
echo "target=go32					">> config.sed
echo "/^version=/ c\					">> config.sed
sed -e "s/[^\"]*\"/version=/" -e "s/;//" version.c	 >> config.sed
echo "s/CC = cc/CC = gcc/				">> config.sed
echo "s/:\$/: \$/g					">> config.sed
echo "s/^	\.\//	go32 /				">> config.sed
echo "s/^	\$(srcdir)\/move-if-change/	update/	">> config.sed
echo "s/^USE_/# USE_/					">> config.sed
echo "s/`echo \$(srcdir)\///g				">> config.sed
echo "s/ | sed 's,\^\\\.\/,,'`//g			">> config.sed
echo "s/^	cd \$(srcdir)[ 	]*;//			">> config.sed

echo "/^cccp.o[ 	]*:/,/-c/ {			">> config.sed
echo "  s/	\$(CC)/	>cccp_o.rf/			">> config.sed
echo "  /-c/ a\						">> config.sed
echo "	$(CC) @cccp_o.rf				">> config.sed
echo "}							">> config.sed

echo "/^gcc.o[ 	]*:/,/-c/ {				">> config.sed
echo "  s/	\$(CC)/	>gcc_o.rf/			">> config.sed
echo "  /-c/ a\						">> config.sed
echo "	$(CC) @gcc_o.rf					">> config.sed
echo "}							">> config.sed

echo "/^gcc[ 	]*:/,/-o/ {				">> config.sed
echo "  s/	\$(CC)/	>gcc.rf/			">> config.sed
echo "  /-o/ a\						">> config.sed
echo "	$(CC) @gcc.rf					">> config.sed
echo "}							">> config.sed

echo "/^cc1[ 	]*:/,/-o/ {				">> config.sed
echo "  s/	\$(CC)/	>cc1.rf/			">> config.sed
echo "  /-o/ a\						">> config.sed
echo "	$(CC) @cc1.rf					">> config.sed
echo "}							">> config.sed

echo "/^cc1plus[ 	]*:/,/-o/ {			">> config.sed
echo "  s/	\$(CC)/	>cc1plus.rf/			">> config.sed
echo "  /-o/ a\						">> config.sed
echo "	$(CC) @cc1plus.rf				">> config.sed
echo "}							">> config.sed

echo "/^cc1obj[ 	]*:/,/-o/ {			">> config.sed
echo "  s/	\$(CC)/	>cc1obj.rf/			">> config.sed
echo "  /-o/ a\						">> config.sed
echo "	$(CC) @cc1obj.rf				">> config.sed
echo "}							">> config.sed

echo "/^enquire[ 	]*:/ s/\$(GCC_PARTS)//g		">> config.sed
echo "/^enquire.o[ 	]*:/ s/\$(GCC_PASSES)//g	">> config.sed
echo "/^GCC_FOR_TARGET =/ c\				">> config.sed
echo "GCC_FOR_TARGET = gcc				">> config.sed

sed -e "s/^\"//" -e "s/\"$//" -e "s/[ 	]*$//" config.sed > config2.sed
sed -f config2.sed Makefile.in >> Makefile
del config.sed
del config2.sed
