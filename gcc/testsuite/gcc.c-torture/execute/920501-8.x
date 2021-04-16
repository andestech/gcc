# Please see Andes Bugzilla #11005 for the details.
if { [istarget "nds32*-*-*"] } {
	# The nds32 mculib toolchains require
	# "-u_printf_float" and "-u_scanf_float" options
	# to fully support printf and scanf functionality.
	# These options are supposed to be harmless to newlib toolchain.
	set additional_flags "-u_printf_float -u_scanf_float"
}

return 0

