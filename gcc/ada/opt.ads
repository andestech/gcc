------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                                  O P T                                   --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--          Copyright (C) 1992-2012, Free Software Foundation, Inc.         --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.                                     --
--                                                                          --
-- As a special exception under Section 7 of GPL version 3, you are granted --
-- additional permissions described in the GCC Runtime Library Exception,   --
-- version 3.1, as published by the Free Software Foundation.               --
--                                                                          --
-- You should have received a copy of the GNU General Public License and    --
-- a copy of the GCC Runtime Library Exception along with this program;     --
-- see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    --
-- <http://www.gnu.org/licenses/>.                                          --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

--  This package contains global flags set by the initialization routine from
--  the command line and referenced throughout the compiler, the binder, or
--  other GNAT tools. The comments indicate which options are used by which
--  programs (GNAT, GNATBIND, GNATLINK, GNATMAKE, GPRMAKE, etc).

--  Some flags are labelled "PROJECT MANAGER". These are used by tools that
--  use the Project Manager. These tools include gnatmake, gnatname, the gnat
--  driver, gnatclean, gprbuild and gprclean.

with Hostparm; use Hostparm;
with Types;    use Types;

pragma Warnings (Off);
--  This package is used also by gnatcoll
with System.Strings; use System.Strings;
with System.WCh_Con; use System.WCh_Con;
pragma Warnings (On);

package Opt is

   ----------------------
   -- Checksum Control --
   ----------------------

   --  Checksums are computed for sources to check for sources being the same
   --  from a compilation point of view (e.g. spelling of identifiers and
   --  white space layout do not count in this computation).

   --  The way the checksum is computed has evolved across the various versions
   --  of GNAT. When gprbuild is called with -m, the checksums must be computed
   --  the same way in gprbuild as it was in the GNAT version of the compiler.
   --  The different ways are

   --    Version 6.4 and later:

   --      The Accumulate_Token_Checksum procedure is called after each numeric
   --      literal and each identifier/keyword. For keywords, Tok_Identifier is
   --      used in the call to Accumulate_Token_Checksum.

   --    Versions 5.04 to 6.3:

   --      For keywords, the token value were used in the call to procedure
   --      Accumulate_Token_Checksum. Type Token_Type did not include Tok_Some.

   --    Versions 5.03:

   --      For keywords, the token value were used in the call to
   --      Accumulate_Token_Checksum. Type Token_Type did not include
   --      Tok_Interface, Tok_Overriding, Tok_Synchronized and Tok_Some.

   --    Versions 5.02 and before:

   --      No calls to procedure Accumulate_Token_Checksum (the checksum
   --      mechanism was introduced in version 5.03).

   --  To signal to the scanner whether Accumulate_Token_Checksum needs to be
   --  called and what versions to call, the following Boolean flags are used:

   Checksum_Accumulate_Token_Checksum : Boolean := True;
   --  GPRBUILD
   --  Set to False by gprbuild when the version of GNAT is 5.02 or before. If
   --  this switch is False, then we do not call Accumulate_Token_Checksum, so
   --  the setting of the following two flags is irrelevant.

   Checksum_GNAT_6_3 : Boolean := False;
   --  GPRBUILD
   --  Set to True by gprbuild when the version of GNAT is 6.3 or before.

   Checksum_GNAT_5_03 : Boolean := False;
   --  GPRBUILD
   --  Set to True by gprbuild when the version of GNAT is 5.03 or before.

   ----------------------------------------------
   -- Settings of Modes for Current Processing --
   ----------------------------------------------

   --  The following mode values represent the current state of processing.
   --  The values set here are the default values. Unless otherwise noted,
   --  the value may be reset in Switch-? with an appropriate switch. In
   --  some cases, the values can also be modified by pragmas, and in the
   --  case of some binder variables, Gnatbind.Scan_Bind_Arg may modify
   --  the default values.

   type Ada_Version_Type is (Ada_83, Ada_95, Ada_2005, Ada_2012);
   pragma Ordered (Ada_Version_Type);
   --  Versions of Ada for Ada_Version below. Note that these are ordered,
   --  so that tests like Ada_Version >= Ada_95 are legitimate and useful.
   --  Think twice before using "="; Ada_Version >= Ada_2012 is more likely
   --  what you want, because it will apply to future versions of the language.

   Ada_Version_Default : constant Ada_Version_Type := Ada_2005;
   pragma Warnings (Off, Ada_Version_Default);
   --  GNAT
   --  Default Ada version if no switch given. The Warnings off is to kill
   --  constant condition warnings.
   --
   --  WARNING: some scripts rely on the format of this line of code. Any
   --  change must be coordinated with the scripts requirements.

   Ada_Version : Ada_Version_Type := Ada_Version_Default;
   --  GNAT
   --  Current Ada version for compiler, as set by configuration pragmas,
   --  compiler switches, or implicitly (to Ada_Version_Runtime) when a
   --  predefined or internal file is compiled.

   Ada_Version_Explicit : Ada_Version_Type := Ada_Version_Default;
   --  GNAT
   --  Like Ada_Version, but does not get set implicitly for predefined
   --  or internal units, so it reflects the Ada version explicitly set
   --  using configuration pragmas or compiler switches (or if neither
   --  appears, it remains set to Ada_Version_Default). This is used in
   --  the rare cases (notably for pragmas Preelaborate_05 and Pure_05/12)
   --  where in the run-time we want the explicit version set.

   Ada_Version_Runtime : Ada_Version_Type := Ada_2012;
   --  GNAT
   --  Ada version used to compile the runtime. Used to set Ada_Version (but
   --  not Ada_Version_Explicit) when compiling predefined or internal units.

   Ada_Final_Suffix : constant String := "final";
   Ada_Final_Name   : String_Ptr := new String'("ada" & Ada_Final_Suffix);
   --  GNATBIND
   --  The name of the procedure that performs the finalization at the end of
   --  execution. This variable may be modified by Gnatbind.Scan_Bind_Arg.

   Ada_Init_Suffix : constant String := "init";
   Ada_Init_Name   : String_Ptr := new String'("ada" & Ada_Init_Suffix);
   --  GNATBIND
   --  The name of the procedure that performs initialization at the start
   --  of execution. This variable may be modified by Gnatbind.Scan_Bind_Arg.

   Ada_Main_Name_Suffix : constant String := "main";
   --  GNATBIND
   --  The suffix for Ada_Main_Name. Defined as a constant here so that it
   --  can be referenced in a uniform manner to create either the default
   --  value of Ada_Main_Name (declared below), or the non-default name
   --  set by Gnatbind.Scan_Bind_Arg.

   Ada_Main_Name : String_Ptr := new String'("ada_" & Ada_Main_Name_Suffix);
   --  GNATBIND
   --  The name of the Ada package generated by the binder (when in Ada mode).
   --  This variable may be modified by Gnatbind.Scan_Bind_Arg.

   Address_Clause_Overlay_Warnings : Boolean := True;
   --  GNAT
   --  Set False to disable address clause warnings

   Address_Is_Private : Boolean := False;
   --  GNAT, GNATBIND
   --  Set True if package System has the line "type Address is private;"

   All_Errors_Mode : Boolean := False;
   --  GNAT
   --  Flag set to force display of multiple errors on a single line and
   --  also repeated error messages for references to undefined identifiers
   --  and certain other repeated error messages. Set by use of -gnatf.

   All_Sources : Boolean := False;
   --  GNATBIND
   --  Set to True to require all source files to be present. This flag is
   --  directly modified by gnatmake to affect the shared binder routines.

   Alternate_Main_Name : String_Ptr := null;
   --  GNATBIND
   --  Set to non null when Bind_Alternate_Main_Name is True. This value
   --  is modified as needed by Gnatbind.Scan_Bind_Arg.

   ASIS_Mode : Boolean := False;
   --  GNAT
   --  Enable semantic checks and tree transformations that are important
   --  for ASIS but that are usually skipped if Operating_Mode is set to
   --  Check_Semantics. This flag does not have the corresponding option to set
   --  it ON. It is set ON when Tree_Output is set ON, it can also be set ON
   --  from the code of GNSA-based tool (a client may need to set ON the
   --  Back_Annotate_Rep_Info flag in this case. At the moment this does not
   --  make very much sense, because GNSA cannot do back annotation).

   Assertions_Enabled : Boolean := False;
   --  GNAT
   --  Enable assertions made using pragma Assert

   Assume_No_Invalid_Values : Boolean := False;
   --  GNAT
   --  Normally, in accordance with (RM 13.9.1 (9-11)) the front end assumes
   --  that values could have invalid representations, unless it can clearly
   --  prove that the values are valid. If this switch is set (by -gnatB or by
   --  pragma Assume_No_Invalid_Values (Off)), then the compiler assumes values
   --  are valid and in range of their representations. This feature is now
   --  fully enabled in the compiler.

   Back_Annotate_Rep_Info : Boolean := False;
   --  GNAT
   --  If set True, enables back annotation of representation information
   --  by gigi, even in -gnatc mode. This is set True by the use of -gnatR
   --  (list representation information) or -gnatt (generate tree). It is
   --  also set true if certain Unchecked_Conversion instantiations require
   --  checking based on annotated values.

   Back_End_Handles_Limited_Types : Boolean;
   --  This flag is set true if the back end can properly handle limited or
   --  other by reference types, and avoid copies. If this flag is False, then
   --  the front end does special expansion for conditional expressions to make
   --  sure that no copy occurs. If the flag is True, then the expansion for
   --  conditional expressions relies on the back end properly handling things.
   --  Currently the default is False for all cases (set in gnat1drv). The
   --  default can be modified using -gnatd.L (sets the flag True).

   Bind_Alternate_Main_Name : Boolean := False;
   --  GNATBIND
   --  True if main should be called Alternate_Main_Name.all.
   --  This variable may be set to True by Gnatbind.Scan_Bind_Arg.

   Bind_Main_Program : Boolean := True;
   --  GNATBIND
   --  Set to False if not binding main Ada program

   Bind_For_Library : Boolean := False;
   --  GNATBIND
   --  Set to True if the binder needs to generate a file designed for building
   --  a library. May be set to True by Gnatbind.Scan_Bind_Arg.

   Bind_Only : Boolean := False;
   --  GNATMAKE, GPRMAKE, GPRBUILD
   --  Set to True to skip compile and link steps
   --  (except when Compile_Only and/or Link_Only are True).

   Blank_Deleted_Lines : Boolean := False;
   --  GNAT, GNATPREP
   --  Output empty lines for each line of preprocessed input that is deleted
   --  in the output, including preprocessor lines starting with a '#'.

   Brief_Output : Boolean := False;
   --  GNAT, GNATBIND
   --  Force brief error messages to standard error, even if verbose mode is
   --  set (so that main error messages go to standard output).

   Build_Bind_And_Link_Full_Project : Boolean := False;
   --  GNATMAKE
   --  Set to True to build, bind and link all the sources of a project file
   --  (switch -B)

   Check_Object_Consistency : Boolean := False;
   --  GNATBIND, GNATMAKE
   --  Set to True to check whether every object file is consistent with
   --  its corresponding ada library information (ALI) file. An object
   --  file is inconsistent with the corresponding ALI file if the object
   --  file does not exist or if it has an older time stamp than the ALI file.
   --  Default above is for GNATBIND. GNATMAKE overrides this default to
   --  True (see Make.Initialize) since we normally do need to check source
   --  consistencies in gnatmake.

   Check_Only : Boolean := False;
   --  GNATBIND
   --  Set to True to do checks only, no output of binder file

   Check_Policy_List : Node_Id := Empty;
   --  GNAT
   --  This points to the list of N_Pragma nodes for Check_Policy pragmas
   --  that are linked through the Next_Pragma fields, with the list being
   --  terminated by Empty. The order is most recently processed first. Note
   --  that Push_Scope and Pop_Scope in Sem_Ch8 save and restore the value
   --  of this variable, implementing the required scope control for pragmas
   --  appearing a declarative part.

   Check_Readonly_Files : Boolean := False;
   --  GNATMAKE
   --  Set to True to check readonly files during the make process

   Check_Source_Files : Boolean := True;
   --  GNATBIND, GNATMAKE
   --  Set to True to enable consistency checking for any source files that
   --  are present (i.e. date must match the date in the library info file).
   --  Set to False for object file consistency check only. This flag is
   --  directly modified by gnatmake, to affect the shared binder routines.

   Check_Switches : Boolean := False;
   --  GNATMAKE, GPRMAKE, GPBUILD
   --  Set to True to check compiler options during the make process

   Check_Unreferenced : Boolean := False;
   --  GNAT
   --  Set to True to enable checking for unreferenced entities other
   --  than formal parameters (for which see Check_Unreferenced_Formals)

   Check_Unreferenced_Formals : Boolean := False;
   --  GNAT
   --  Set True to check for unreferenced formals. This is turned on by
   --  -gnatwa/wf/wu and turned off by -gnatwA/wF/wU.

   Check_Withs : Boolean := False;
   --  GNAT
   --  Set to True to enable checking for unused withs, and also the case
   --  of withing a package and using none of the entities in the package.

   CodePeer_Mode : Boolean := False;
   --  GNAT, GNATBIND
   --  Enable full CodePeer mode (SCIL generation, disable switches that
   --  interact badly with it, etc...).

   Commands_To_Stdout : Boolean := False;
   --  GNATMAKE
   --  True if echoed commands to be written to stdout instead of stderr

   Comment_Deleted_Lines : Boolean := False;
   --  GNATPREP
   --  True if source lines removed by the preprocessor should be commented
   --  in the output file.

   Compile_Only : Boolean := False;
   --  GNATMAKE, GNATCLEAN, GPRMAKE, GPBUILD, GPRCLEAN
   --  GNATMAKE, GPRMAKE, GPRMAKE:
   --    set to True to skip bind and link steps (except when Bind_Only is
   --    True).
   --  GNATCLEAN, GPRCLEAN:
   --    set to True to delete only the files produced by the compiler but not
   --    the library files or the executable files.

   Config_File : Boolean := True;
   --  GNAT
   --  Set to False to inhibit reading and processing of gnat.adc file

   Config_File_Names : String_List_Access := null;
   --  GNAT
   --  Names of configuration pragmas files (given by switches -gnatec)

   Configurable_Run_Time_Mode : Boolean := False;
   --  GNAT, GNATBIND
   --  Set True if the compiler is operating in configurable run-time mode.
   --  This happens if the flag Targparm.Configurable_Run_TimeMode_On_Target
   --  is set True, or if pragma No_Run_Time is used. See the spec of Rtsfind
   --  for details on the handling of the latter pragma.

   Constant_Condition_Warnings : Boolean := False;
   --  GNAT
   --  Set to True to activate warnings on constant conditions

   Create_Mapping_File : Boolean := False;
   --  GNATMAKE, GPRMAKE
   --  Set to True (-C switch) to indicate that the compiler will be invoked
   --  with a mapping file (-gnatem compiler switch).

   Debug_Pragmas_Enabled : Boolean := False;
   --  GNAT
   --  Enable debug statements from pragma Debug

   Debug_Pragmas_Disabled : Boolean := False;
   --  GNAT
   --  Debug pragmas completely disabled (no semantic checking)

   subtype Debug_Level_Value is Nat range 0 .. 3;
   Debugger_Level : Debug_Level_Value := 0;
   --  GNATBIND
   --  The value given to the -g parameter. The default value for -g with
   --  no value is 2. This is usually ignored by GNATBIND, except in the
   --  VMS version where it is passed as an argument to __gnat_initialize
   --  to trigger the activation of the remote debugging interface.
   --  Is this still true ???

   Debug_Generated_Code : Boolean := False;
   --  GNAT
   --  Set True (-gnatD switch) to debug generated expanded code instead
   --  of the original source code. Causes debugging information to be
   --  written with respect to the generated code file that is written.

   Default_Exit_Status : Int := 0;
   --  GNATBIND
   --  Set the default exit status value. Set by the -Xnnn switch for the
   --  binder.

   Default_Stack_Size : Int := -1;
   --  GNATBIND
   --  Set to default primary stack size in units of bytes. Set by
   --  the -dnnn switch for the binder. A value of -1 indicates that no
   --  default was set by the binder.

   Default_Sec_Stack_Size : Int := -1;
   --  GNATBIND
   --  Set to default secondary stack size in units of bytes. Set by
   --  the -Dnnn switch for the binder. A value of -1 indicates that no
   --  default was set by the binder, and that the default should be the
   --  initial value of System.Secondary_Stack.Default_Secondary_Stack_Size.

   Default_Pool : Node_Id := Empty;
   --  GNAT
   --  Used to record the storage pool name (or null literal) that is the
   --  argument of an applicable pragma Default_Storage_Pool.
   --    Empty:       No pragma Default_Storage_Pool applies.
   --    N_Null node: "pragma Default_Storage_Pool (null);" applies.
   --    otherwise:   "pragma Default_Storage_Pool (X);" applies, and
   --                 this points to the name X.
   --  Push_Scope and Pop_Scope in Sem_Ch8 save and restore this value.

   Detect_Blocking : Boolean := False;
   --  GNAT
   --  Set True to force the run time to raise Program_Error if calls to
   --  potentially blocking operations are detected from protected actions.

   Display_Compilation_Progress : Boolean := False;
   --  GNATMAKE, GPRMAKE, GPRBUILD
   --  Set True (-d switch) to display information on progress while compiling
   --  files. Internal flag to be used in conjunction with an IDE (e.g GPS).

   type Distribution_Stub_Mode_Type is
   --  GNAT
     (No_Stubs,
      --  Normal mode, no generation/compilation of distribution stubs

      Generate_Receiver_Stub_Body,
      --  The unit being compiled is the RCI body, and the compiler will
      --  generate the body for the receiver stubs and compile it.

      Generate_Caller_Stub_Body);
      --  The unit being compiled is the RCI spec, and the compiler will
      --  generate the body for the caller stubs and compile it.

   Distribution_Stub_Mode : Distribution_Stub_Mode_Type := No_Stubs;
   --  GNAT
   --  This enumeration variable indicates the five states of distribution
   --  annex stub generation/compilation.

   Do_Not_Execute : Boolean := False;
   --  GNATMAKE
   --  Set to True if no actual compilations should be undertaken.

   Dump_Source_Text : Boolean := False;
   --  GNAT
   --  Set to True (by -gnatL) to dump source text intermingled with generated
   --  code. Effective only if either of Debug/Print_Generated_Code is true.

   Dynamic_Elaboration_Checks : Boolean := False;
   --  GNAT
   --  Set True for dynamic elaboration checking mode, as set by the -gnatE
   --  switch or by the use of pragma Elaboration_Checks (Dynamic).

   Dynamic_Stack_Measurement : Boolean := False;
   --  GNATBIND
   --  Set True to enable dynamic stack measurement (-u flag for gnatbind)

   Dynamic_Stack_Measurement_Array_Size : Nat := 100;
   --  GNATBIND
   --  Number of measurements we want to store during dynamic stack analysis.
   --  When the buffer is full, non-storable results will be output on the fly.
   --  The value is relevant only if Dynamic_Stack_Measurement is set. Set
   --  by processing of -u flag for gnatbind.

   Elab_Dependency_Output : Boolean := False;
   --  GNATBIND
   --  Set to True to output complete list of elaboration constraints

   Elab_Order_Output : Boolean := False;
   --  GNATBIND
   --  Set to True to output chosen elaboration order

   Elab_Warnings : Boolean := False;
   --  GNAT
   --  Set to True to generate full elaboration warnings (-gnatwl)

   Error_Msg_Line_Length : Nat := 0;
   --  GNAT
   --  Records the error message line length limit. If this is set to zero,
   --  then we get the old style behavior, in which each call to the error
   --  message routines generates one line of output as a separate message.
   --  If it is set to a non-zero value, then continuation lines are folded
   --  to make a single long message, and then this message is split up into
   --  multiple lines not exceeding the specified length. Set by -gnatj=nn.

   Exception_Handler_Encountered : Boolean := False;
   --  GNAT
   --  This flag is set true if the parser encounters an exception handler.
   --  It is used to set Warn_On_Exception_Propagation True if the restriction
   --  No_Exception_Propagation is set.

   Exception_Extra_Info : Boolean := False;
   --  GNAT
   --  True when switch -gnateE is used. When True, generate extra information
   --  associated with exception messages (in particular range and index
   --  checks).

   Exception_Locations_Suppressed : Boolean := False;
   --  GNAT
   --  Set to True if a Suppress_Exception_Locations configuration pragma is
   --  currently active.

   type Exception_Mechanism_Type is
   --  Determines the handling of exceptions. See Exp_Ch11 for details
   --
     (Front_End_Setjmp_Longjmp_Exceptions,
      --  Exceptions use setjmp/longjmp generated explicitly by the
      --  front end (this includes gigi or other equivalent parts of
      --  the code generator). AT END handlers are converted into
      --  exception handlers by the front end in this mode.

      Back_End_Exceptions);
      --  Exceptions are handled by the back end. The front end simply
      --  generates the handlers as they appear in the source, and AT
      --  END handlers are left untouched (they are not converted into
      --  exception handlers when operating in this mode.
   pragma Convention (C, Exception_Mechanism_Type);

   Exception_Mechanism : Exception_Mechanism_Type :=
                           Front_End_Setjmp_Longjmp_Exceptions;
   --  GNAT
   --  Set to the appropriate value depending on the default as given in
   --  system.ads (ZCX_By_Default). The C convention is there to make this
   --  variable accessible to gigi.

   Exception_Tracebacks : Boolean := False;
   --  GNATBIND
   --  Set to True to store tracebacks in exception occurrences (-E)

   Extensions_Allowed : Boolean := False;
   --  GNAT
   --  Set to True by switch -gnatX if GNAT specific language extensions
   --  are allowed. For example, the use of 'Constrained with objects of
   --  generic types is a GNAT extension.

   type External_Casing_Type is (
     As_Is,       -- External names cased as they appear in the Ada source
     Uppercase,   -- External names forced to all uppercase letters
     Lowercase);  -- External names forced to all lowercase letters

   External_Name_Imp_Casing : External_Casing_Type := Lowercase;
   --  GNAT
   --  The setting of this flag determines the casing of external names
   --  when the name is implicitly derived from an entity name (i.e. either
   --  no explicit External_Name or Link_Name argument is used, or, in the
   --  case of extended DEC pragmas, the external name is given using an
   --  identifier. The As_Is setting is not permitted here (since this would
   --  create Ada source programs that were case sensitive).

   External_Name_Exp_Casing : External_Casing_Type := As_Is;
   --  GNAT
   --  The setting of this flag determines the casing of an external name
   --  specified explicitly with a string literal. As_Is means the string
   --  literal is used as given with no modification to the casing. If
   --  Lowercase or Uppercase is set, then the string is forced to all
   --  lowercase or all uppercase letters as appropriate. Note that this
   --  setting has no effect if the external name is given using an identifier
   --  in the case of extended DEC import/export pragmas (in this case the
   --  casing is controlled by External_Name_Imp_Casing), and also has no
   --  effect if an explicit Link_Name is supplied (a link name is always
   --  used exactly as given).

   External_Unit_Compilation_Allowed : Boolean := False;
   --  GNATMAKE
   --  When True (set by gnatmake switch -x), allow compilation of sources
   --  that are not part of any project file.

   Fast_Math : Boolean := False;
   --  GNAT
   --  Indicates the current setting of Fast_Math mode, as set by the use
   --  of a Fast_Math pragma (set on by Fast_Math (On)).

   Float_Format : Character := ' ';
   --  GNAT
   --  A non-blank value indicates that a Float_Format pragma has been
   --  processed, in which case this variable is set to 'I' for IEEE or
   --  to 'V' for VAX. The setting of 'V' is only possible on OpenVMS
   --  versions of GNAT.

   Float_Format_Long : Character := ' ';
   --  GNAT
   --  A non-blank value indicates that a Long_Float pragma has been
   --  processed (this pragma is recognized only in OpenVMS versions
   --  of GNAT), in which case this variable is set to D or G for
   --  D_Float or G_Float.

   Force_ALI_Tree_File : Boolean := False;
   --  GNAT
   --  Force generation of ALI file even if errors are encountered.
   --  Also forces generation of tree file if -gnatt is also set.

   Disable_ALI_File : Boolean := False;
   --  GNAT
   --  Disable generation of ALI file

   Force_Checking_Of_Elaboration_Flags : Boolean := False;
   --  GNATBIND
   --  True if binding with forced checking of the elaboration flags
   --  (-F switch set).

   Force_Compilations : Boolean := False;
   --  GNATMAKE, GPRMAKE, GPRBUILD
   --  Set to force recompilations even when the objects are up-to-date.

   Full_Path_Name_For_Brief_Errors : Boolean := False;
   --  PROJECT MANAGER
   --  When True, in Brief_Output mode, each error message line
   --  will start with the full path name of the source.
   --  When False, only the file name without directory information
   --  is used.

   Full_List : Boolean := False;
   --  GNAT
   --  Set True to generate full source listing with embedded errors

   Full_List_File_Name : String_Ptr := null;
   --  GNAT
   --  Set to file name to generate full source listing to named file (or if
   --  the name is of the form .xxx, then to name.xxx where name is the source
   --  file name with extension stripped.

   Generate_Processed_File : Boolean := False;
   --  GNAT
   --  True when switch -gnateG is used. When True, create in a file
   --  <source>.prep, if the source is preprocessed.

   Generate_SCO : Boolean := False;
   --  GNAT
   --  True when switch -gnateS is used. When True, Source Coverage Obligation
   --  (SCO) information is generated and output in the ALI file. See unit
   --  Par_SCO for full details.

   Generating_Code : Boolean := False;
   --  GNAT
   --  True if the frontend finished its work and has called the backend to
   --  process the tree and generate the object file.

   Global_Discard_Names : Boolean := False;
   --  GNAT, GNATBIND
   --  True if a pragma Discard_Names appeared as a configuration pragma for
   --  the current compilation unit.

   GNAT_Mode : Boolean := False;
   --  GNAT
   --  True if compiling in GNAT system mode (-gnatg switch)

   Heap_Size : Nat := 0;
   --  GNATBIND
   --  Heap size for memory allocations. Valid values are 32 and 64. Only
   --  available on VMS.

   Identifier_Character_Set : Character;
   --  GNAT
   --  This variable indicates the character set to be used for identifiers.
   --  The possible settings are:
   --    '1'  Latin-5 (ISO-8859-1)
   --    '2'  Latin-5 (ISO-8859-2)
   --    '3'  Latin-5 (ISO-8859-3)
   --    '4'  Latin-5 (ISO-8859-4)
   --    '5'  Latin-5 (ISO-8859-5, Cyrillic)
   --    '9'  Latin-5 (ISO-8859-9)
   --    'p'  PC (US, IBM page 437)
   --    '8'  PC (European, IBM page 850)
   --    'f'  Full upper set (all distinct)
   --    'n'  No upper characters (Ada 83 rules)
   --    'w'  Latin-1 plus wide characters allowed in identifiers
   --
   --  The setting affects the set of letters allowed in identifiers and the
   --  upper/lower case equivalences. It does not affect the interpretation of
   --  character and string literals, which are always stored using the actual
   --  coding in the source program. This variable is initialized to the
   --  default value appropriate to the system (in Osint.Initialize), and then
   --  reset if a command line switch is used to change the setting.

   Ignore_Rep_Clauses : Boolean := False;
   --  GNAT
   --  Set True to ignore all representation clauses. Useful when compiling
   --  code from foreign compilers for checking or ASIS purposes. Can be
   --  set True by use of -gnatI.

   Implementation_Unit_Warnings : Boolean := True;
   --  GNAT
   --  Set True to active warnings for use of implementation internal units.
   --  Can be controlled by use of -gnatwi/-gnatwI.

   Implicit_Packing : Boolean := False;
   --  GNAT
   --  If set True, then a Size attribute clause on an array is allowed to
   --  cause implicit packing instead of generating an error message. Set by
   --  use of pragma Implicit_Packing.

   Ineffective_Inline_Warnings : Boolean := False;
   --  GNAT
   --  Set True to activate warnings if front-end inlining (-gnatN) is not
   --  able to actually inline a particular call (or all calls). Can be
   --  controlled by use of -gnatwp/-gnatwP.

   Init_Or_Norm_Scalars : Boolean := False;
   --  GNAT, GANTBIND
   --  Set True if a pragma Initialize_Scalars applies to the current unit.
   --  Also set True if a pragma Normalize_Scalars applies.

   Initialize_Scalars : Boolean := False;
   --  GNAT
   --  Set True if a pragma Initialize_Scalars applies to the current unit.
   --  Note that Init_Or_Norm_Scalars is also set to True if this is True.

   Initialize_Scalars_Mode1 : Character := 'I';
   Initialize_Scalars_Mode2 : Character := 'N';
   --  GNATBIND
   --  Set to two characters from -S switch (IN/LO/HI/EV/xx). The default
   --  is IN (invalid values), used if no -S switch is used.

   Inline_Active : Boolean := False;
   --  GNAT
   --  Set True to activate pragma Inline processing across modules. Default
   --  for now is not to inline across module boundaries.

   Inline_Level : Nat := 0;
   --  GNAT
   --  Set to indicate the inlining level: 0 means that an appropriate value is
   --  to be computed by the compiler based on the optimization level (-gnatn),
   --  1 is for moderate inlining across modules (-gnatn1) and 2 for full
   --  inlining across modules (-gnatn2).

   Interface_Library_Unit : Boolean := False;
   --  GNATBIND
   --  Set to True to indicate that at least one ALI file is an interface ALI:
   --  then elaboration flag checks are to be generated in the binder
   --  generated file.

   Generate_SCIL : Boolean := False;
   --  GNAT
   --  Set True to activate SCIL code generation.

   Invalid_Value_Used : Boolean := False;
   --  GNAT
   --  Set True if a valid Invalid_Value attribute is encountered

   Follow_Links_For_Files : Boolean := False;
   --  PROJECT MANAGER
   --  Set to True (-eL) to process the project files in trusted mode. If
   --  Follow_Links is False, it is assumed that the project doesn't contain
   --  any file duplicated through symbolic links (although the latter are
   --  still valid if they point to a file which is outside of the project),
   --  and that no directory has a name which is a valid source name.

   Follow_Links_For_Dirs : Boolean := False;
   --  PROJECT MANAGER
   --  Set to True if directories can be links in this project, and therefore
   --  additional system calls must be performed to ensure that we always see
   --  the same full name for each directory.

   Front_End_Inlining : Boolean := False;
   --  GNAT
   --  Set True to activate inlining by front-end expansion

   Inline_Processing_Required : Boolean := False;
   --  GNAT
   --  Set True if inline processing is required. Inline processing is required
   --  if an active Inline pragma is processed. The flag is set for a pragma
   --  Inline or Inline_Always that is actually active.

   In_Place_Mode : Boolean := False;
   --  GNATMAKE
   --  Set True to store ALI and object files in place i.e. in the object
   --  directory if these files already exist or in the source directory
   --  if not.

   Keep_Going : Boolean := False;
   --  GNATMAKE, GPRMAKE, GPRBUILD
   --  When True signals to ignore compilation errors and keep processing
   --  sources until there is no more work.

   Keep_Temporary_Files : Boolean := False;
   --  GNATCMD
   --  When True the temporary files created by the GNAT driver are not
   --  deleted. Set by switch -dn or qualifier /KEEP_TEMPORARY_FILES.

   Leap_Seconds_Support : Boolean := False;
   --  GNATBIND
   --  Set to True to enable leap seconds support in Ada.Calendar and its
   --  children.

   Link_Only : Boolean := False;
   --  GNATMAKE, GPRMAKE, GPRBUILD
   --  Set to True to skip compile and bind steps (except when Bind_Only is
   --  set to True).

   List_Inherited_Aspects : Boolean := False;
   --  GNAT
   --  List inherited invariants, preconditions, and postconditions from
   --  Invariant'Class, Pre'Class, and Post'Class aspects. Also list inherited
   --  subtype predicates. Set True by use of -gnatw.l and False by use of
   --  -gnatw.L.

   List_Restrictions : Boolean := False;
   --  GNATBIND
   --  Set to True to list restrictions pragmas that could apply to partition

   List_Units : Boolean := False;
   --  GNAT
   --  List units in the active library for a compilation (-gnatu switch)

   List_Closure : Boolean := False;
   --  GNATBIND
   --  List all sources in the closure of a main (-R gnatbind switch)

   List_Dependencies : Boolean := False;
   --  GNATMAKE
   --  When True gnatmake verifies that the objects are up to date and
   --  outputs the list of object dependencies (-M switch).
   --  Output depends if -a switch is used or not.
   --  This list can be used directly in a Makefile.

   List_Representation_Info : Int range 0 .. 3 := 0;
   --  GNAT
   --  Set non-zero by -gnatR switch to list representation information.
   --  The settings are as follows:
   --
   --    0 = no listing of representation information (default as above)
   --    1 = list rep info for user defined record and array types
   --    2 = list rep info for all user defined types and objects
   --    3 = like 2, but variable fields are decoded symbolically

   List_Representation_Info_To_File : Boolean := False;
   --  GNAT
   --  Set true by -gnatRs switch. Causes information from -gnatR/1/2/3 to be
   --  written to file.rep (where file is the name of the source file) instead
   --  of stdout. For example, if file x.adb is compiled using -gnatR2s then
   --  representation info is written to x.adb.ref.

   List_Representation_Info_Mechanisms : Boolean := False;
   --  GNAT
   --  Set true by -gnatRm switch. Causes information on mechanisms to be
   --  included in the representation output information.

   List_Preprocessing_Symbols : Boolean := False;
   --  GNAT, GNATPREP
   --  Set to True if symbols for preprocessing a source are to be listed
   --  before preprocessing occurs. Set to True by switch -s of gnatprep or
   --  -s in preprocessing data file for the compiler.

   type Create_Repinfo_File_Proc is access procedure (Src  : String);
   type Write_Repinfo_Line_Proc  is access procedure (Info : String);
   type Close_Repinfo_File_Proc  is access procedure;
   --  Types used for procedure addresses below

   Create_Repinfo_File_Access : Create_Repinfo_File_Proc := null;
   Write_Repinfo_Line_Access  : Write_Repinfo_Line_Proc  := null;
   Close_Repinfo_File_Access  : Close_Repinfo_File_Proc  := null;
   --  GNAT
   --  These three locations are left null when operating in non-compiler (e.g.
   --  ASIS mode), but when operating in compiler mode, they are set to point
   --  to the three corresponding procedures in Osint-C. The reason for this
   --  slightly strange interface is to stop Repinfo from dragging in Osint in
   --  ASIS mode, which would include lots of unwanted units in the ASIS build.

   type Create_List_File_Proc is access procedure (S : String);
   type Write_List_Info_Proc  is access procedure (S : String);
   type Close_List_File_Proc  is access procedure;
   --  Types used for procedure addresses below

   Create_List_File_Access : Create_List_File_Proc := null;
   Write_List_Info_Access  : Write_List_Info_Proc  := null;
   Close_List_File_Access  : Close_List_File_Proc  := null;
   --  GNAT
   --  These three locations are left null when operating in non-compiler
   --  (e.g. from the binder), but when operating in compiler mode, they are
   --  set to point to the three corresponding procedures in Osint-C. The
   --  reason for this slightly strange interface is to prevent Repinfo
   --  from dragging in Osint-C in the binder, which would include unwanted
   --  units in the binder.

   Locking_Policy : Character := ' ';
   --  GNAT, GNATBIND
   --  Set to ' ' for the default case (no locking policy specified). Reset to
   --  first character (uppercase) of locking policy name if a valid pragma
   --  Locking_Policy is encountered.

   Locking_Policy_Sloc : Source_Ptr := No_Location;
   --  GNAT, GNATBIND
   --  Remember location of previous Locking_Policy pragma. This is used for
   --  inconsistency error messages. A value of System_Location is used if the
   --  policy is set in package System.

   Look_In_Primary_Dir : Boolean := True;
   --  GNAT, GNATBIND, GNATMAKE, GNATCLEAN
   --  Set to False if a -I- was present on the command line. When True we are
   --  allowed to look in the primary directory to locate other source or
   --  library files.

   Make_Steps : Boolean := False;
   --  GNATMAKE
   --  Set to True when either Compile_Only, Bind_Only or Link_Only is
   --  set to True.

   Main_Index : Int := 0;
   --  GNATMAKE
   --  This is set to non-zero by gnatmake switch -eInnn to indicate that
   --  the main program is the nnn unit in a multi-unit source file.

   Mapping_File_Name : String_Ptr := null;
   --  GNAT
   --  File name of mapping between unit names, file names and path names.
   --  (given by switch -gnatem)

   Maximum_Messages : Int := 9999;
   --  GNAT, GNATBIND
   --  Maximum default number of errors before compilation is terminated, or in
   --  the case of GNAT, maximum number of warnings before further warnings are
   --  suppressed. Can be overridden by -gnatm (GNAT) or -m (GNATBIND) switch.

   Maximum_File_Name_Length : Int;
   --  GNAT, GNATBIND
   --  Maximum number of characters allowed in a file name, not counting the
   --  extension, as set by the appropriate switch. If no switch is given,
   --  then this value is initialized by Osint to the appropriate value.

   Maximum_Instantiations : Int := 8000;
   --  GNAT
   --  Maximum number of instantiations permitted (to stop runaway cases
   --  of nested instantiations). These situations probably only occur in
   --  specially concocted test cases. Can be modified by -gnateinn switch.

   Maximum_Processes : Positive := 1;
   --  GNATMAKE, GPRMAKE, GPRBUILD
   --  Maximum number of processes that should be spawned to carry out
   --  compilations.

   Minimal_Recompilation : Boolean := False;
   --  GNATMAKE
   --  Set to True if minimal recompilation mode requested

   Multiple_Unit_Index : Int;
   --  GNAT
   --  This is set non-zero if the current unit is being compiled in multiple
   --  unit per file mode, meaning that the current unit is selected from the
   --  sequence of units in the current source file, using the value stored
   --  in this variable (e.g. 2 = select second unit in file). A value of
   --  zero indicates that we are in normal (one unit per file) mode.

   No_Main_Subprogram : Boolean := False;
   --  GNATMAKE, GNATBIND
   --  Set to True if compilation/binding of a program without main
   --  subprogram requested.

   No_Run_Time_Mode : Boolean := False;
   --  GNAT, GNATBIND
   --  This flag is set True if a No_Run_Time pragma is encountered. See
   --  spec of Rtsfind for a full description of handling of this pragma.

   No_Split_Units : Boolean := False;
   --  GPRBUILD
   --  Set to True with switch --no-split-units. When True, unit sources, spec,
   --  body and subunits, must all be in the same project.This is checked after
   --  each compilation.

   No_Stdinc : Boolean := False;
   --  GNAT, GNATBIND, GNATMAKE, GNATFIND, GNATXREF
   --  Set to True if no default source search dirs added to search list

   No_Stdlib : Boolean := False;
   --  GNATMAKE, GNATBIND, GNATFIND, GNATXREF
   --  Set to True if no default library search dirs added to search list

   No_Strict_Aliasing : Boolean := False;
   --  GNAT
   --  Set True if pragma No_Strict_Aliasing with no parameters encountered

   Normalize_Scalars : Boolean := False;
   --  GNAT, GNATBIND
   --  Set True if a pragma Normalize_Scalars applies to the current unit.
   --  Note that Init_Or_Norm_Scalars is also set to True if this is True.

   Object_Directory_Present : Boolean := False;
   --  GNATMAKE
   --  Set to True when an object directory is specified with option -D

   Object_Path_File_Name : String_Ptr := null;
   --  GNAT2WHY
   --  Path of the temporary file that contains a list of object directories
   --  passed by -gnateO=<obj_pat_file>.

   One_Compilation_Per_Obj_Dir : Boolean := False;
   --  GNATMAKE, GPRBUILD
   --  Set to True with switch --single-compile-per-obj-dir. When True, there
   --  cannot be simultaneous compilations with the object files in the same
   --  object directory, if project files are used.

   type Operating_Mode_Type is (Check_Syntax, Check_Semantics, Generate_Code);
   Operating_Mode : Operating_Mode_Type := Generate_Code;
   --  GNAT
   --  Indicates the operating mode of the compiler. The default is generate
   --  code, which runs the parser, semantics and backend. Switches can be
   --  used to set syntax checking only mode, or syntax and semantics checking
   --  only mode. Operating_Mode can also be modified as a result of detecting
   --  errors during the compilation process. In particular if any serious
   --  error is detected then this flag is reset from Generate_Code to
   --  Check_Semantics after generating an error message.

   Optimize_Alignment : Character := 'O';
   --  Setting of Optimize_Alignment, set to T/S/O for time/space/off. Can
   --  be modified by use of pragma Optimize_Alignment.

   Optimize_Alignment_Local : Boolean := False;
   --  Set True if Optimize_Alignment mode is set by a local configuration
   --  pragma that overrides the gnat.adc (or other configuration file) default
   --  so that the unit is not dependent on the default setting. Also always
   --  set True for internal units, since these always have a default setting
   --  of Optimize_Alignment (Off) that is enforced (essentially equivalent to
   --  them all having such an explicit pragma in each unit).

   Original_Operating_Mode : Operating_Mode_Type := Generate_Code;
   --  GNAT
   --  Indicates the original operating mode of the compiler as set by compiler
   --  options. This is identical to Operating_Mode except that this is not
   --  affected by errors.

   Optimization_Level : Int;
   pragma Import (C, Optimization_Level, "optimize");
   --  Constant reflecting the optimization level (0,1,2,3 for -O0,-O1,-O2,-O3)
   --  See jmissing.c and aamissing.c for definitions for dotnet/jgnat and
   --  GNAAMP back ends.

   Optimize_Size : Int;
   pragma Import (C, Optimize_Size, "optimize_size");
   --  Constant reflecting setting of -Os (optimize for size). Set to nonzero
   --  in -Os mode and set to zero otherwise. See jmissing.c and aamissing.c
   --  for definitions of "optimize_size" for dotnet/jgnat and GNAAMP backends

   Output_File_Name_Present : Boolean := False;
   --  GNATBIND, GNAT, GNATMAKE, GPRMAKE
   --  Set to True when the output C file name is given with option -o for
   --  GNATBIND, when the object file name is given with option -gnatO for GNAT
   --  or when the executable is given with option -o for GNATMAKE or GPRMAKE.

   Output_Linker_Option_List : Boolean := False;
   --  GNATBIND
   --  True if output of list of linker options is requested (-K switch set)

   Output_ALI_List   : Boolean := False;
   ALI_List_Filename : String_Ptr;
   --  GNATBIND
   --  True if output of list of ALIs is requested (-A switch set). List is
   --  output under the given filename, or standard output if not specified.

   Output_Object_List   : Boolean := False;
   Object_List_Filename : String_Ptr;
   --  GNATBIND
   --  True if output of list of objects is requested (-O switch set). List is
   --  output under the given filename, or standard output if not specified.

   Persistent_BSS_Mode : Boolean := False;
   --  GNAT
   --  True if a Persistent_BSS configuration pragma is in effect, causing
   --  potentially persistent data to be placed in the persistent_bss section.

   Pessimistic_Elab_Order : Boolean := False;
   --  GNATBIND
   --  True if pessimistic elaboration order is to be chosen (-p switch set)

   Polling_Required : Boolean := False;
   --  GNAT
   --  Set to True if polling for asynchronous abort is enabled by using
   --  the -gnatP option for GNAT.

   Preprocessing_Data_File : String_Ptr := null;
   --  GNAT
   --  Set by switch -gnatep=. The file name of the preprocessing data file.

   Preprocessing_Symbol_Defs : String_List_Access := new String_List (1 .. 4);
   --  An extensible array to temporarily stores symbol definitions specified
   --  on the command line with -gnateD switches.
   --  What is this magic constant 4 ???
   --  What is extensible about this fixed length array ???

   Preprocessing_Symbol_Last : Natural := 0;
   --  Index of last symbol definition in array Symbol_Definitions

   Print_Generated_Code : Boolean := False;
   --  GNAT
   --  Set to True to enable output of generated code in source form. This
   --  flag is set by the -gnatG switch.

   Print_Standard : Boolean := False;
   --  GNAT
   --  Set to true to enable printing of package standard in source form.
   --  This flag is set by the -gnatS switch

   Propagate_Exceptions : Boolean := False;
   --  GNAT
   --  Indicates if subprogram descriptor exception tables should be
   --  built for imported subprograms. Set True if a Propagate_Exceptions
   --  pragma applies to the extended main unit.

   type Usage is (Unknown, Not_In_Use, In_Use);
   Project_File_In_Use : Usage := Unknown;
   --  GNAT
   --  Indicates if a project file is used or not. Set to In_Use by the first
   --  SFNP pragma.

   Queuing_Policy : Character := ' ';
   --  GNAT, GNATBIND
   --  Set to ' ' for the default case (no queuing policy specified). Reset to
   --  first character (uppercase) of locking policy name if a valid
   --  Queuing_Policy pragma is encountered.

   Queuing_Policy_Sloc : Source_Ptr := No_Location;
   --  GNAT, GNATBIND
   --  Remember location of previous Queuing_Policy pragma. This is used for
   --  inconsistency error messages. A value of System_Location is used if the
   --  policy is set in package System.

   Quiet_Output : Boolean := False;
   --  GNATMAKE, GNATCLEAN, GPRMAKE, GPRBUILD, GPRCLEAN
   --  Set to True if the tool should not have any output if there are no
   --  errors or warnings.

   Replace_In_Comments : Boolean := False;
   --  GNATPREP
   --  Set to True if -C switch used

   RTS_Lib_Path_Name : String_Ptr := null;
   RTS_Src_Path_Name : String_Ptr := null;
   --  GNAT
   --  Set to the "adalib" and "adainclude" directories of the run time
   --  specified by --RTS=.

   RTS_Switch : Boolean := False;
   --  GNAT, GNATMAKE, GNATBIND, GNATLS, GNATFIND, GNATXREF
   --  Set to True when the --RTS switch is set

   Run_Path_Option : Boolean := True;
   --  GNATMAKE, GNATLINK
   --  Set to False when no run_path_option should be issued to the linker

   Search_Directory_Present : Boolean := False;
   --  GNAT
   --  Set to True when argument is -I. Reset to False when next argument, a
   --  search directory path is taken into account. Note that this is quite
   --  different from other switches in this section in that it is only set in
   --  a transitory manner as a result of scanning a -I switch with no file
   --  name, and if set, is an indication that the next argument is to be
   --  treated as a file name.

   Sec_Stack_Used : Boolean := False;
   --  GNAT, GBATBIND
   --  Set True if generated code uses the System.Secondary_Stack package. For
   --  the binder, set if any unit uses the secondary stack package.

   Setup_Projects : Boolean := False;
   --  GNAT DRIVER
   --  Set to True for GNAT SETUP: the Project Manager creates non existing
   --  object, library and exec directories.

   Shared_Libgnat : Boolean;
   --  GNATBIND
   --  Set to True if a shared libgnat is requested by using the -shared option
   --  for GNATBIND and to False when using the -static option. The value of
   --  this flag is set by Gnatbind.Scan_Bind_Arg.

   Short_Circuit_And_Or : Boolean := False;
   --  GNAT
   --  Set True if a pragma Short_Circuit_And_Or applies to the current unit.

   Short_Descriptors : Boolean := False;
   --  GNAT
   --  Set True if a pragma Short_Descriptors applies to the current unit.

   Special_Exception_Package_Used : Boolean := False;
   --  GNAT
   --  Set to True if either of the unit GNAT.Most_Recent_Exception or
   --  GNAT.Exception_Traces is with'ed. Used to inhibit transformation of
   --  local raise statements into gotos in the presence of either package.

   Sprint_Line_Limit : Nat := 72;
   --  GNAT
   --  Limit values for chopping long lines in Sprint output, can be reset
   --  by use of NNN parameter with -gnatG or -gnatD switches.

   Stack_Checking_Enabled : Boolean;
   --  GNAT
   --  Set to indicate if -fstack-check switch is set for the compilation. True
   --  means that the switch is set, so that stack checking is enabled. False
   --  means that the switch is not set (no stack checking). This value is
   --  obtained from the external imported value flag_stack_check in the gcc
   --  backend (see Frontend) and may be referenced throughout the compilation
   --  phases.

   Style_Check : Boolean := False;
   --  GNAT
   --  Set True to perform style checks. Activates checks carried out in
   --  package Style (see body of this package for details of checks). This
   --  flag is set True by either the -gnatg or -gnaty switches.

   Suppress_All_Inlining : Boolean := False;
   --  GNAT
   --  Set by -fno-inline. Suppresses all inlining, both front end and back end
   --  regardless of any other switches that are set.

   Suppress_Control_Flow_Optimizations : Boolean := False;
   --  GNAT
   --  Set by -fpreserve-control-flow. Suppresses control flow optimizations
   --  that interfere with coverage analysis based on the object code.

   System_Extend_Pragma_Arg : Node_Id := Empty;
   --  GNAT
   --  Set non-empty if and only if a correct Extend_System pragma was present
   --  in which case it points to the argument of the pragma, and the name can
   --  be located as Chars (Expression (System_Extend_Pragma_Arg)).

   System_Extend_Unit : Node_Id := Empty;
   --  GNAT
   --  This is set to Empty if GNAT_Mode is set, since pragma Extend_System
   --  is never appropriate in GNAT_Mode (and causes troubles, including
   --  bogus circularities, if we try to compile the run-time library with
   --  a System extension). If GNAT_Mode is not set, then System_Extend_Unit
   --  is a copy of the value set in System_Extend_Pragma_Arg.

   Subunits_Missing : Boolean := False;
   --  GNAT
   --  This flag is set true if missing subunits are detected with code
   --  generation active. This causes code generation to be skipped.

   Suppress_Checks : Boolean := False;
   --  GNAT
   --  Set to True if -gnatp (suppress all checks) switch present.

   Suppress_Options : Suppress_Record;
   --  GNAT
   --  Indicates outer level setting of check suppression. This initializes
   --  the settings of the outer scope level in any unit compiled. This is
   --  initialized by Osint.Initialize, and further initialized by the
   --  Adjust_Global_Switches flag in Gnat1drv.

   Suppress_Back_Annotation : Boolean := False;
   --  GNAT
   --  This flag is set True if back annotation of representation information
   --  is to be suppressed. This is set if neither -gnatt or -gnatR0-3 is set.
   --  This avoids unnecessary time being spent on back annotation.

   Table_Factor : Int := 1;
   --  GNAT
   --  Factor by which all initial table sizes set in Alloc are multiplied.
   --  Used in Table to calculate initial table sizes (the initial table size
   --  is the value in Alloc, used as the Table_Initial parameter value,
   --  multiplied by the factor given here. The default value is used if no
   --  -gnatT switch appears.

   Tagged_Type_Expansion : Boolean := True;
   --  GNAT
   --  Set True if tagged types and interfaces should be expanded by the
   --  front-end. If False, the original tree is left unexpanded for tagged
   --  types and dispatching calls, assuming the underlying target supports
   --  it (e.g. in the JVM case).

   Task_Dispatching_Policy : Character := ' ';
   --  GNAT, GNATBIND
   --  Set to ' ' for the default case (no task dispatching policy specified).
   --  Reset to first character (uppercase) of task dispatching policy name
   --  if a valid Task_Dispatching_Policy pragma is encountered.

   Task_Dispatching_Policy_Sloc : Source_Ptr := No_Location;
   --  GNAT, GNATBIND
   --  Remember location of previous Task_Dispatching_Policy pragma. This is
   --  used for inconsistency error messages. A value of System_Location is
   --  used if the policy is set in package System.

   Tasking_Used : Boolean := False;
   --  Set True if any tasking construct is encountered. Used to activate the
   --  output of the Q, L and T lines in ALI files.

   Time_Slice_Set : Boolean := False;
   --  GNATBIND
   --  Set True if a pragma Time_Slice is processed in the main unit, or
   --  if the -gnatTnn switch is present to set a time slice value.

   Time_Slice_Value : Nat;
   --  GNATBIND
   --  Time slice value. Valid only if Time_Slice_Set is True, i.e. if
   --  Time_Slice pragma has been processed. Set to the time slice value in
   --  microseconds. Negative values are stored as zero, and the value is not
   --  larger than 1_000_000_000 (1000 seconds). Values larger than this are
   --  reset to this maximum. This can also be set with the -gnatTnn switch.

   Tolerate_Consistency_Errors : Boolean := False;
   --  GNATBIND
   --  Tolerate time stamp and other consistency errors. If this flag is set to
   --  True (-t), then inconsistencies result in warnings rather than errors.

   Treat_Categorization_Errors_As_Warnings : Boolean := False;
   --  Normally categorization errors are true illegalities. If this switch
   --  is set, then such errors result in warning messages rather than error
   --  messages. Set True by -gnateP (P for Pure/Preelaborate).

   Treat_Restrictions_As_Warnings : Boolean := False;
   --  GNAT
   --  Set True to treat pragma Restrictions as Restriction_Warnings. Set by
   --  -gnatr switch.

   Tree_Output : Boolean := False;
   --  GNAT
   --  Set to True (-gnatt) to generate output tree file

   True_VMS_Target : Boolean := False;
   --  Set True if we are on a VMS target. The setting of this flag reflects
   --  the true state of the compile, unlike Targparm.OpenVMS_On_Target which
   --  can also be true when debug flag m is set (-gnatdm). This is used in the
   --  few cases where we do NOT want -gnatdm to trigger the VMS behavior.

   Try_Semantics : Boolean := False;
   --  GNAT
   --  Flag set to force attempt at semantic analysis, even if parser errors
   --  occur. This will probably cause blowups at this stage in the game. On
   --  the other hand, most such blowups will be caught cleanly and simply
   --  say compilation abandoned. This flag is set to True by -gnatq or -gnatQ.

   Unchecked_Shared_Lib_Imports : Boolean := False;
   --  GPRBUILD
   --  Set to True when shared library projects are allowed to import projects
   --  that are not shared library projects. Set by switch
   --  --unchecked-shared-lib-imports.

   Undefined_Symbols_Are_False : Boolean := False;
   --  GNAT, GNATPREP
   --  Set to True by switch -u of gnatprep or -u in the preprocessing data
   --  file for the compiler. Indicates that while preprocessing sources,
   --  symbols that are not defined have the value FALSE.

   Unique_Error_Tag : Boolean := Tag_Errors;
   --  GNAT
   --  Indicates if error messages are to be prefixed by the string error:
   --  Initialized from Tag_Errors, can be forced on with the -gnatU switch.

   Universal_Addressing_On_AAMP : Boolean := False;
   --  GNAAMP
   --  Indicates if library-level objects should be accessed and updated using
   --  universal addressing instructions on the AAMP architecture. This flag is
   --  set to True when pragma Universal_Data is given as a configuration
   --  pragma.

   Unreserve_All_Interrupts : Boolean := False;
   --  GNAT, GNATBIND
   --  Normally set False, set True if a valid Unreserve_All_Interrupts pragma
   --  appears anywhere in the main unit for GNAT, or if any ALI file has the
   --  corresponding attribute set in GNATBIND.

   Upper_Half_Encoding : Boolean := False;
   --  GNAT, GNATBIND
   --  Normally set False, indicating that upper half ISO 8859-1 characters are
   --  used in the normal way to represent themselves. If the wide character
   --  encoding method uses the upper bit for this encoding, then this flag is
   --  set True, and upper half characters in the source indicate the start of
   --  a wide character sequence. Set by -gnatW or -W switches.

   Use_Include_Path_File : Boolean := False;
   --  GNATMAKE, GPRBUILD
   --  When True, create a source search path file, even when a mapping file
   --  is used.

   Usage_Requested : Boolean := False;
   --  GNAT, GNATBIND, GNATMAKE
   --  Set to True if -h (-gnath for the compiler) switch encountered
   --  requesting usage information

   Use_Expression_With_Actions : Boolean;
   --  The N_Expression_With_Actions node has been introduced relatively
   --  recently, and not all back ends are prepared to handle it yet. So
   --  we use this flag to suppress its use during a transitional period.
   --  Currently the default is False for all cases (set in gnat1drv).
   --  The default can be modified using -gnatd.X/-gnatd.Y.

   Use_Pragma_Linker_Constructor : Boolean := False;
   --  GNATBIND
   --  True if pragma Linker_Constructor applies to adainit

   Use_VADS_Size : Boolean := False;
   --  GNAT
   --  Set to True if a valid pragma Use_VADS_Size is processed

   Validity_Checks_On  : Boolean := True;
   --  GNAT
   --  This flag determines if validity checking is on or off. The initial
   --  state is on, and the required default validity checks are active. The
   --  actual set of checks that is performed if Validity_Checks_On is set is
   --  defined by the switches in package Validsw. The Validity_Checks_On flag
   --  is controlled by pragma Validity_Checks (On | Off), and also some
   --  generated compiler code (typically code that has to do with validity
   --  check generation) is compiled with this flag set to False. This flag is
   --  set to False by the -gnatp switch.

   Verbose_Mode : Boolean := False;
   --  GNAT, GNATBIND, GNATMAKE, GNATLINK, GNATLS, GNATNAME, GNATCLEAN,
   --  GPRMAKE, GPRBUILD, GPRCLEAN
   --  Set to True to get verbose mode (full error message text and location
   --  information sent to standard output, also header, copyright and summary)

   type Verbosity_Level_Type is (None, Low, Medium, High);
   pragma Ordered (Verbosity_Level_Type);
   Verbosity_Level : Verbosity_Level_Type := High;
   --  GNATMAKE, GPRMAKE
   --  Modified by gnatmake or gprmake switches -v, -vl, -vm, -vh. Indicates
   --  the level of verbosity of informational messages:
   --
   --  In Low Verbosity, the reasons why a source is recompiled, the name
   --  of the executable and the reason it must be rebuilt is output.
   --
   --  In Medium Verbosity, additional lines are output for each ALI file
   --  that is checked.
   --
   --  In High Verbosity, additional lines are output when the ALI file
   --  is part of an Ada library, is read-only or is part of the runtime.

   Warn_On_Ada_2005_Compatibility : Boolean := True;
   --  GNAT
   --  Set to True to generate all warnings on Ada 2005 compatibility issues,
   --  including warnings on Ada 2005 obsolescent features used in Ada 2005
   --  mode. Set False by -gnatwY.

   Warn_On_Ada_2012_Compatibility : Boolean := True;
   --  GNAT
   --  Set to True to generate all warnings on Ada 2012 compatibility issues,
   --  including warnings on Ada 2012 obsolescent features used in Ada 2012
   --  mode. Set False by -gnatwY.

   Warn_On_All_Unread_Out_Parameters : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings in all cases where a variable is
   --  modified by being passed as to an OUT formal, but the resulting value is
   --  never read. The default is that this warning is suppressed, except in
   --  the case of

   Warn_On_Assertion_Failure : Boolean := True;
   --  GNAT
   --  Set to True to activate warnings on assertions that can be determined
   --  at compile time will always fail. Set false by -gnatw.A.

   Warn_On_Assumed_Low_Bound : Boolean := True;
   --  GNAT
   --  Set to True to activate warnings for string parameters that are indexed
   --  with literals or S'Length, presumably assuming a lower bound of one. Set
   --  False by -gnatwW.

   Warn_On_Atomic_Synchronization : Boolean := False;
   --  GNAT
   --  Set to True to generate information messages for atomic synchronization.
   --  Set True by use of -gnatw.n.

   Warn_On_Bad_Fixed_Value : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for static fixed-point expression
   --  values that are not an exact multiple of the small value of the type.

   Warn_On_Biased_Representation : Boolean := True;
   --  GNAT
   --  Set to True to generate warnings for size clauses, component clauses
   --  and component_size clauses that force biased representation. Set False
   --  by -gnatw.B.

   Warn_On_Constant : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for variables that could be declared
   --  as constants. Modified by use of -gnatwk/K.

   Warn_On_Deleted_Code : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for code deleted by the front end
   --  for conditional statements whose outcome is known at compile time.
   --  Modified by use of -gnatwt/T.

   Warn_On_Dereference : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for implicit dereferences for array
   --  indexing and record component access. Modified by use of -gnatwd/D.

   Warn_On_Export_Import : Boolean := True;
   --  GNAT
   --  Set to True to generate warnings for suspicious use of export or
   --  import pragmas. Modified by use of -gnatwx/X.

   Warn_On_Hiding : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings if a declared entity hides another
   --  entity. The default is that this warning is suppressed.

   Warn_On_Modified_Unread : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings if a variable is assigned but is never
   --  read. Also controls warnings for similar cases involving out parameters,
   --  but only if there is only one out parameter for the procedure involved.
   --  The default is that this warning is suppressed.

   Warn_On_No_Value_Assigned : Boolean := True;
   --  GNAT
   --  Set to True to generate warnings if no value is ever assigned to a
   --  variable that is at least partially uninitialized. Set to false to
   --  suppress such warnings. The default is that such warnings are enabled.

   Warn_On_Non_Local_Exception : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for non-local exception raises and also
   --  handlers that can never handle a local raise. This warning is only ever
   --  generated if pragma Restrictions (No_Exception_Propagation) is set. The
   --  default is not to generate the warnings except that if the source has
   --  at least one exception handler, and this restriction is set, and the
   --  warning was not explicitly turned off, then it is turned on by default.

   No_Warn_On_Non_Local_Exception : Boolean := False;
   --  GNAT
   --  This is set to True if the above warning is explicitly suppressed. We
   --  use this to avoid turning it on by default when No_Exception_Propagation
   --  restriction is set and an exception handler is present.

   Warn_On_Object_Renames_Function : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings when a function result is renamed as
   --  an object. The default is that this warning is disabled.

   Warn_On_Obsolescent_Feature : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings on use of any feature in Annex or if a
   --  subprogram is called for which a pragma Obsolescent applies.

   Warn_On_Overlap : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings when a writable actual which is not
   --  a by-copy type overlaps with another actual in a subprogram call.

   Warn_On_Questionable_Missing_Parens : Boolean := True;
   --  GNAT
   --  Set to True to generate warnings for cases where parentheses are missing
   --  and the usage is questionable, because the intent is unclear.

   Warn_On_Parameter_Order : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for cases where the argument list for
   --  a call is a sequence of identifiers that match the formal identifiers,
   --  but are in the wrong order.

   Warn_On_Redundant_Constructs : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for redundant constructs (e.g. useless
   --  assignments/conversions). The default is that this warning is disabled.

   Warn_On_Reverse_Bit_Order : Boolean := True;
   --  GNAT
   --  Set to True to generate warning (informational) messages for component
   --  clauses that are affected by non-standard bit-order. The default is
   --  that this warning is enabled.

   Warn_On_Suspicious_Contract : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for suspicious contracts expressed as
   --  pragmas or aspects precondition and postcondition. The default is that
   --  this warning is disabled.

   Warn_On_Suspicious_Modulus_Value : Boolean := True;
   --  GNAT
   --  Set to True to generate warnings for suspicious modulus values. The
   --  default is that this warning is enabled.

   Warn_On_Unchecked_Conversion : Boolean := True;
   --  GNAT
   --  Set to True to generate warnings for unchecked conversions that may have
   --  non-portable semantics (e.g. because sizes of types differ). The default
   --  is that this warning is enabled.

   Warn_On_Unordered_Enumeration_Type : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for inappropriate uses (comparisons
   --  and explicit ranges) on unordered enumeration types (which includes
   --  all enumeration types for which pragma Ordered is not given). The
   --  default is that this warning is disabled.

   Warn_On_Unrecognized_Pragma : Boolean := True;
   --  GNAT
   --  Set to True to generate warnings for unrecognized pragmas. The default
   --  is that this warning is enabled.

   Warn_On_Unrepped_Components : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for the case of components of record
   --  which have a record representation clause but this component does not
   --  have a component clause. The default is that this warning is disabled.

   Warn_On_Warnings_Off : Boolean := False;
   --  GNAT
   --  Set to True to generate warnings for use of Pragma Warnings (Off, ent),
   --  where either the pragma is never used, or it could be replaced by a
   --  pragma Unmodified or Unreferenced.

   type Warning_Mode_Type is (Suppress, Normal, Treat_As_Error);
   Warning_Mode : Warning_Mode_Type := Normal;
   --  GNAT, GNATBIND
   --  Controls treatment of warning messages. If set to Suppress, warning
   --  messages are not generated at all. In Normal mode, they are generated
   --  but do not count as errors. In Treat_As_Error mode, warning messages
   --  are generated and are treated as errors.

   Wide_Character_Encoding_Method : WC_Encoding_Method := WCEM_Brackets;
   --  GNAT, GNATBIND
   --  Method used for encoding wide characters in the source program. See
   --  description of type in unit System.WCh_Con for a list of the methods
   --  that are currently supported. Note that brackets notation is always
   --  recognized in source programs regardless of the setting of this
   --  variable. The default setting causes only the brackets notation to be
   --  recognized. If this is the main unit, this setting also controls the
   --  output of the W=? parameter in the ALI file, which is used to provide
   --  the default for encoding [Wide_[Wide_]]Text_IO files. For the binder,
   --  the value set here overrides this main unit default.

   Wide_Character_Encoding_Method_Specified : Boolean := False;
   --  GNAT, GNATBIND
   --  Set True if the value in Wide_Character_Encoding_Method was set as
   --  a result of an explicit -gnatW? or -W? switch. False otherwise.

   Xref_Active : Boolean := True;
   --  GNAT
   --  Set if cross-referencing is enabled (i.e. xref info in ALI files)

   Zero_Formatting : Boolean := False;
   --  GNATBIND
   --  Do no formatting (no title, no leading spaces, no empty lines) in
   --  auxiliary outputs (-e, -K, -l, -R).

   ----------------------------
   -- Configuration Settings --
   ----------------------------

   --  These are settings that are used to establish the mode at the start of
   --  each unit. The values defined below can be affected either by command
   --  line switches, or by the use of appropriate configuration pragmas in a
   --  configuration pragma file.

   Ada_Version_Config : Ada_Version_Type;
   --  GNAT
   --  This is the value of the configuration switch for the Ada 83 mode, as
   --  set by the command line switches -gnat83/95/05, and possibly modified by
   --  the use of configuration pragmas Ada_*. This switch is used to set the
   --  initial value for Ada_Version mode at the start of analysis of a unit.
   --  Note however that the setting of this flag is ignored for internal and
   --  predefined units (which are always compiled in the most up to date
   --  version of Ada).

   Ada_Version_Explicit_Config : Ada_Version_Type;
   --  GNAT
   --  This is set in the same manner as Ada_Version_Config. The difference is
   --  that the setting of this flag is not ignored for internal and predefined
   --  units, which for some purposes do indeed access this value, regardless
   --  of the fact that they are compiled the most up to date ada version).

   Assertions_Enabled_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch for assertions enabled
   --  mode, as possibly set by the command line switch -gnata, and possibly
   --  modified by the use of the configuration pragma Assertion_Policy.

   Assume_No_Invalid_Values_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch for assuming "no invalid
   --  values enabled" mode, as possibly set by the command line switch
   --  -gnatB, and possibly modified by the use of the configuration pragma
   --  Assume_No_Invalid_Values.

   Check_Policy_List_Config : Node_Id;
   --  GNAT
   --  This points to the list of N_Pragma nodes for Check_Policy pragmas
   --  that are linked through the Next_Pragma fields, with the list being
   --  terminated by Empty. The order is most recently processed first. This
   --  list includes only those pragmas in configuration pragma files.

   Debug_Pragmas_Disabled_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch for debug pragmas disabled
   --  mode, as possibly set by use of the configuration pragma Debug_Policy.

   Debug_Pragmas_Enabled_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch for debug pragmas enabled
   --  mode, as possibly set by the command line switch -gnata and possibly
   --  modified by the use of the configuration pragma Debug_Policy.

   Default_Pool_Config : Node_Id := Empty;
   --  GNAT
   --  Same as Default_Pool above, except this is only for Default_Storage_Pool
   --  pragmas that are configuration pragmas.

   Dynamic_Elaboration_Checks_Config : Boolean := False;
   --  GNAT
   --  Set True for dynamic elaboration checking mode, as set by the -gnatE
   --  switch or by the use of pragma Elaboration_Checking (Dynamic).

   Exception_Locations_Suppressed_Config : Boolean := False;
   --  GNAT
   --  Set True by use of the configuration pragma Suppress_Exception_Messages

   Extensions_Allowed_Config : Boolean;
   --  GNAT
   --  This is the flag that indicates whether extensions are allowed. It can
   --  be set True either by use of the -gnatX switch, or by use of the
   --  configuration pragma Extensions_Allowed (On). It is always set to True
   --  for internal GNAT units, since extensions are always permitted in such
   --  units.

   External_Name_Exp_Casing_Config : External_Casing_Type;
   --  GNAT
   --  This is the value of the configuration switch that controls casing of
   --  external symbols for which an explicit external name is given. It can be
   --  set to Uppercase by the command line switch -gnatF, and further modified
   --  by the use of the configuration pragma External_Name_Casing in the
   --  gnat.adc file. This flag is used to set the initial value for
   --  External_Name_Exp_Casing at the start of analyzing each unit. Note
   --  however that the setting of this flag is ignored for internal and
   --  predefined units (which are always compiled with As_Is mode).

   External_Name_Imp_Casing_Config : External_Casing_Type;
   --  GNAT
   --  This is the value of the configuration switch that controls casing of
   --  external symbols where the external name is implicitly given. It can be
   --  set to Uppercase by the command line switch -gnatF, and further modified
   --  by the use of the configuration pragma External_Name_Casing in the
   --  gnat.adc file. This flag is used to set the initial value for
   --  External_Name_Imp_Casing at the start of analyzing each unit. Note
   --  however that the setting of this flag is ignored for internal and
   --  predefined units (which are always compiled with Lowercase mode).

   Fast_Math_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch that controls Fast_Math
   --  mode, as set by a Fast_Math pragma in configuration pragmas. It is
   --  used to set the initial value of Fast_Math at the start of each new
   --  compilation unit.

   Initialize_Scalars_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch that is set by the
   --  pragma Initialize_Scalars when it appears in the gnat.adc file.
   --  This switch is not set when the pragma appears ahead of a given
   --  unit, so it does not affect the compilation of other units.

   Optimize_Alignment_Config : Character;
   --  GNAT
   --  This is the value of the configuration switch that controls the
   --  alignment optimization mode, as set by an Optimize_Alignment pragma.
   --  It is used to set the initial value of Optimize_Alignment at the start
   --  of each new compilation unit, except that it is always set to 'O' (off)
   --  for internal units.

   Persistent_BSS_Mode_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch that controls whether
   --  potentially persistent data is to be placed in the persistent_bss
   --  section. It can be set True by use of the pragma Persistent_BSS.
   --  This flag is used to set the initial value of Persistent_BSS_Mode
   --  at the start of each compilation unit, except that it is always
   --  set False for predefined units.

   Polling_Required_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch that controls polling
   --  mode. It can be set True by the command line switch -gnatP, and then
   --  further modified by the use of pragma Polling in the gnat.adc file. This
   --  flag is used to set the initial value for Polling_Required at the start
   --  of analyzing each unit.

   Short_Descriptors_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch that controls the use of
   --  Short_Descriptors for setting descriptor default sizes. It can be set
   --  True by the use of the pragma Short_Descriptors in the gnat.adc file.
   --  This flag is used to set the initial value for Short_Descriptors at the
   --  start of analyzing each unit.

   Use_VADS_Size_Config : Boolean;
   --  GNAT
   --  This is the value of the configuration switch that controls the use of
   --  VADS_Size instead of Size wherever the attribute Size is used. It can
   --  be set True by the use of the pragma Use_VADS_Size in the gnat.adc file.
   --  This flag is used to set the initial value for Use_VADS_Size at the
   --  start of analyzing each unit. Note however that the setting of this flag
   --  is ignored for internal and predefined units (which are always compiled
   --  with the standard Size semantics).

   type Config_Switches_Type is private;
   --  Type used to save values of the switches set from Config values

   procedure Save_Opt_Config_Switches (Save : out Config_Switches_Type);
   --  This procedure saves the current values of the switches which are
   --  initialized from the above Config values, and then resets these switches
   --  according to the Config value settings.

   procedure Set_Opt_Config_Switches
     (Internal_Unit : Boolean;
      Main_Unit     : Boolean);
   --  This procedure sets the switches to the appropriate initial values. The
   --  parameter Internal_Unit is True for an internal or predefined unit, and
   --  affects the way the switches are set (see above). Main_Unit is true if
   --  switches are being set for the main unit (this affects setting of the
   --  assert/debug pragma switches, which are normally set false by default
   --  for an internal unit, except when the internal unit is the main unit,
   --  in which case we use the command line settings).

   procedure Restore_Opt_Config_Switches (Save : Config_Switches_Type);
   --  This procedure restores a set of switch values previously saved by a
   --  call to Save_Opt_Switches.

   procedure Register_Opt_Config_Switches;
   --  This procedure is called after processing the gnat.adc file and other
   --  configuration pragma files to record the values of the Config switches,
   --  as possibly modified by the use of command line switches and pragmas
   --  appearing in these files.

   ------------------------
   -- Other Global Flags --
   ------------------------

   Expander_Active : Boolean := False;
   --  A flag that indicates if expansion is active (True) or deactivated
   --  (False). When expansion is deactivated all calls to expander routines
   --  have no effect. Note that the initial setting of False is merely to
   --  prevent saving of an undefined value for an initial call to the
   --  Expander_Mode_Save_And_Set procedure. For more information on the use of
   --  this flag, see package Expander. Indeed this flag might more logically
   --  be in the spec of Expander, but it is referenced by Errout, and it
   --  really seems wrong for Errout to depend on Expander.
   --
   --  Note: for many purposes, it is more appropriate to test the flag
   --  Full_Expander_Active, which also checks that Alfa mode is not active.

   Static_Dispatch_Tables : Boolean := True;
   --  This flag indicates if the backend supports generation of statically
   --  allocated dispatch tables. If it is True, then the front end will
   --  generate static aggregates for dispatch tables that contain forward
   --  references to addresses of subprograms not seen yet, and the back end
   --  must be prepared to handle this case. If it is False, then the front
   --  end generates assignments to initialize the dispatch table, and there
   --  are no such forward references. By default we build statically allocated
   --  dispatch tables for all library level tagged types in all platforms.This
   --  behavior can be disabled using switch -gnatd.t which will set this flag
   --  to False and revert to the previous dynamic behavior.

   -----------------------
   -- Tree I/O Routines --
   -----------------------

   procedure Tree_Read;
   --  Reads switch settings from current tree file using Tree_Read

   procedure Tree_Write;
   --  Writes out switch settings to current tree file using Tree_Write

   --------------------------
   -- ASIS Version Control --
   --------------------------

   --  These two variables (Tree_Version_String and Tree_ASIS_Version_Number)
   --  are supposed to be used in the GNAT/ASIS version check performed in
   --  the ASIS code (this package is also a part of the ASIS implementation).
   --  They are set by Tree_Read procedure, so they represent the version
   --  number (and the version string) of the compiler which has created the
   --  tree, and they are supposed to be compared with the corresponding values
   --  from the Tree_IO and Gnatvsn packages which also are a part of ASIS
   --  implementation.

   Tree_Version_String : String_Access;
   --  Used to store the compiler version string read from a tree file to check
   --  if it is from the same date as stored in the version string in Gnatvsn.
   --  We require that ASIS Pro can be used only with GNAT Pro, but we allow
   --  non-Pro ASIS and ASIS-based tools to be used with any version of the
   --  GNAT compiler. Therefore, we need the possibility to compare the dates
   --  of the corresponding source sets, using version strings that may be
   --  of different lengths.

   Tree_ASIS_Version_Number : Int;
   --  Used to store the ASIS version number read from a tree file to check if
   --  it is the same as stored in the ASIS version number in Tree_IO.

   -----------------------------------
   -- Modes for Formal Verification --
   -----------------------------------

   Alfa_Mode : Boolean := False;
   --  Specific compiling mode targeting formal verification through the
   --  generation of Why code for those parts of the input code that belong to
   --  the Alfa subset of Ada. Set by debug flag -gnatd.F.

   Strict_Alfa_Mode : Boolean := False;
   --  Interpret compiler permissions as strictly as possible. E.g. base ranges
   --  for integers are limited to the strict minimum with this option. Set by
   --  debug flag -gnatd.D.

   function Full_Expander_Active return Boolean;
   pragma Inline (Full_Expander_Active);
   --  Returns the value of (Expander_Active and not Alfa_Mode). This "flag"
   --  indicates that expansion is fully active, that is, not in the reduced
   --  mode for Alfa (True) or that expansion is either deactivated, or active
   --  in the reduced mode for Alfa (False). For more information on full
   --  expansion, see package Expander. For more information on reduced
   --  Alfa expansion, see package Exp_Alfa.

private

   --  The following type is used to save and restore settings of switches in
   --  Opt that represent the configuration (i.e. result of config pragmas).

   --  Note that Ada_Version_Explicit is not included, since this is a sticky
   --  flag that once set does not get reset, since the whole idea of this flag
   --  is to record the setting for the main unit.

   type Config_Switches_Type is record
      Ada_Version                    : Ada_Version_Type;
      Ada_Version_Explicit           : Ada_Version_Type;
      Assertions_Enabled             : Boolean;
      Assume_No_Invalid_Values       : Boolean;
      Check_Policy_List              : Node_Id;
      Debug_Pragmas_Disabled         : Boolean;
      Debug_Pragmas_Enabled          : Boolean;
      Default_Pool                   : Node_Id;
      Dynamic_Elaboration_Checks     : Boolean;
      Exception_Locations_Suppressed : Boolean;
      Extensions_Allowed             : Boolean;
      External_Name_Exp_Casing       : External_Casing_Type;
      External_Name_Imp_Casing       : External_Casing_Type;
      Fast_Math                      : Boolean;
      Initialize_Scalars             : Boolean;
      Normalize_Scalars              : Boolean;
      Optimize_Alignment             : Character;
      Optimize_Alignment_Local       : Boolean;
      Persistent_BSS_Mode            : Boolean;
      Polling_Required               : Boolean;
      Short_Descriptors              : Boolean;
      Use_VADS_Size                  : Boolean;
   end record;

   --  The following declarations are for GCC version dependent flags. We do
   --  not let client code in the compiler test GCC_Version directly, but
   --  instead use deferred constants for relevant feature tags.

   --  Note: there currently are no such constants defined in this section,
   --  since the compiler front end is currently entirely independent of the
   --  GCC version, which is a desirable state of affairs.

   function get_gcc_version return Int;
   pragma Import (C, get_gcc_version, "get_gcc_version");

   GCC_Version : constant Nat := get_gcc_version;
   --  GNATMAKE
   --  Indicates which version of gcc is in use (3 = 3.x, 4 = 4.x). Note that
   --  gcc 2.8.1 (which used to be a value of 2) is no longer supported.

end Opt;
