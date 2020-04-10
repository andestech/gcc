/* Common hooks for RISC-V.
   Copyright (C) 2016-2020 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include <sstream>

#define INCLUDE_STRING
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "common/common-target.h"
#include "common/common-target-def.h"
#include "opts.h"
#include "flags.h"
#include "diagnostic-core.h"
#include "config/riscv/riscv-protos.h"

#define RISCV_DONT_CARE_VERSION -1

#define MAX_ARCH_STRING_LEN 256
static char riscv_arch_string[MAX_ARCH_STRING_LEN];

#define NUM_EXTS_KIND 3

struct arch_options_t
{
  const char *ext;
  const char *option_name;
  bool is_spec;
  bool val;
  unsigned default_major_version;
  unsigned default_minor_version;
};

static arch_options_t std_ext_options[] = {
  {"a", "atomic",     false, false, 2, 0},
  {"c", "16-bit",     false, false, 2, 0},
  {"v", "ext-vector", false, false, 1, 0},
  {"p",  "ext-dsp",   false, false, 0, 5},
  {NULL, NULL, false, false, 2, 0}
};

static arch_options_t nonstd_z_ext_options[] = {
  {"zfh", "zfh", false, false, 0, 1},
  {"zba", "ext-zbabcs", false, false, 1, 0},
  {"zbb", "ext-zbabcs",  false, false, 1, 0},
  {"zbc", "ext-zbabcs", false, false, 1, 0},
  {"zbs", "ext-zbabcs", false, false, 1, 0},
  {NULL, NULL, false, false, 0, 5}
};

static arch_options_t nonstd_x_ext_options[] = {
  {"xandes",    "nds", false, false, 5, 0},
  {"xefhw",    "fp16", false, false, 2, 0},
  {"xebfhw",    "bf16", false, false, 2, 0},
  {NULL, NULL, false, false, 0, 5}
};

static arch_options_t *ext_options[] = {
  std_ext_options,
  nonstd_z_ext_options,
  nonstd_x_ext_options
};

static bool
arch_options_end_p (const arch_options_t *opt)
{
  return (opt->ext == NULL) && (opt->option_name == NULL);
}

static bool
arch_options_disabled_p(const arch_options_t *opt, const char *p)
{
  for (; !arch_options_end_p(opt); ++opt)
    if (strncmp(opt->ext, p, strlen(opt->ext)) == 0 &&
	opt->is_spec && !opt->val)
	return true;

  return false;
}

static bool
arch_options_enabled_p(const arch_options_t *opt, const char *p)
{
  for (; !arch_options_end_p(opt); ++opt)
    if (strncmp(opt->ext, p, strlen(opt->ext)) == 0 &&
	opt->is_spec && opt->val)
	return true;

  return false;
}

static void arch_options_default_version(const arch_options_t *opt, const char *p,
					 unsigned *major_version,
					 unsigned *minor_version)
{
  for (; !arch_options_end_p(opt); ++opt)
    if (strncmp(opt->ext, p, strlen(opt->ext)) == 0)
      {
	*major_version = opt->default_major_version;
	*minor_version = opt->default_minor_version;
      }
}

/* Subset info.  */
struct riscv_subset_t
{
  riscv_subset_t ();

  std::string name;
  int major_version;
  int minor_version;
  struct riscv_subset_t *next;
};

/* Type for implied ISA info.  */
struct riscv_implied_info_t
{
  const char *ext;
  const char *implied_ext;
};

/* Implied ISA info, must end with NULL sentinel.  */
riscv_implied_info_t riscv_implied_info[] =
{
  {"d", "f"},
  {NULL, NULL}
};

/* Subset list.  */
class riscv_subset_list
{
private:
  /* Original arch string.  */
  const char *m_arch;

  /* Location of arch string, used for report error.  */
  location_t m_loc;

  /* Head of subset info list.  */
  riscv_subset_t *m_head;

  /* Tail of subset info list.  */
  riscv_subset_t *m_tail;

  /* X-len of m_arch. */
  unsigned m_xlen;

  riscv_subset_list (const char *, location_t);

  const char *parsing_subset_version (const char *, unsigned *, unsigned *,
				      unsigned, unsigned, bool);

  const char *parse_std_ext (const char *);

  const char *parse_multiletter_ext (const char *, const char *,
				     const char *,
				     const arch_options_t *opt = NULL);

  void handle_implied_ext (riscv_subset_t *);

public:
  ~riscv_subset_list ();

  void add (const char *, int, int);

  void add (const char *, const char *);

  void add (const char *, unsigned, unsigned, const arch_options_t *opt);

  riscv_subset_t *lookup (const char *,
			  int major_version = RISCV_DONT_CARE_VERSION,
			  int minor_version = RISCV_DONT_CARE_VERSION) const;

  std::string to_string (bool) const;

  unsigned xlen() const {return m_xlen;};

  static riscv_subset_list *parse (const char *, location_t);

};

static const char *riscv_supported_std_ext (void);

static const char *riscv_convert_nds_ext (const char *);

static riscv_subset_list *current_subset_list = NULL;

const riscv_subset_list *riscv_current_subset_list ()
{
  return current_subset_list;
}

riscv_subset_t::riscv_subset_t ()
  : name (), major_version (0), minor_version (0), next (NULL)
{
}

riscv_subset_list::riscv_subset_list (const char *arch, location_t loc)
  : m_arch (arch), m_loc (loc), m_head (NULL), m_tail (NULL), m_xlen (0)
{
}

riscv_subset_list::~riscv_subset_list ()
{
  if (!m_head)
    return;

  riscv_subset_t *item = this->m_head;
  while (item != NULL)
    {
      riscv_subset_t *next = item->next;
      delete item;
      item = next;
    }
}

/* Get the rank for single-letter subsets, lower value meaning higher
   priority.  */

static int
single_letter_subset_rank (char ext)
{
  int rank;

  switch (ext)
    {
    case 'i':
      return 0;
    case 'e':
      return 1;
    default:
      break;
    }

  const char *all_ext = riscv_supported_std_ext ();
  const char *ext_pos = strchr (all_ext, ext);
  if (ext_pos == NULL)
    /* If got an unknown extension letter, then give it an alphabetical
       order, but after all known standard extension.  */
    rank = strlen (all_ext) + ext - 'a';
  else
    rank = (int)(ext_pos - all_ext) + 2 /* e and i has higher rank.  */;

  return rank;
}

/* Get the rank for multi-letter subsets, lower value meaning higher
   priority.  */

static int
multi_letter_subset_rank (const std::string &subset)
{
  gcc_assert (subset.length () >= 2);
  int high_order = -1;
  int low_order = 0;
  /* The order between multi-char extensions: s -> h -> z -> x.  */
  char multiletter_class = subset[0];
  switch (multiletter_class)
    {
    case 's':
      high_order = 0;
      break;
    case 'h':
      high_order = 1;
      break;
    case 'z':
      gcc_assert (subset.length () > 2);
      high_order = 2;
      break;
    case 'x':
      high_order = 3;
      break;
    default:
      gcc_unreachable ();
      return -1;
    }

  if (multiletter_class == 'z')
    /* Order for z extension on spec: If multiple "Z" extensions are named, they
       should be ordered first by category, then alphabetically within a
       category - for example, "Zicsr_Zifencei_Zam". */
    low_order = single_letter_subset_rank (subset[1]);
  else
    low_order = 0;

  return (high_order << 8) + low_order;
}

/* subset compare

  Returns an integral value indicating the relationship between the subsets:
  Return value  indicates
  -1            B has higher order than A.
  0             A and B are same subset.
  1             A has higher order than B.

*/

static int
subset_cmp (const std::string &a, const std::string &b)
{
  if (a == b)
    return 0;

  size_t a_len = a.length ();
  size_t b_len = b.length ();

  /* Single-letter extension always get higher order than
     multi-letter extension.  */
  if (a_len == 1 && b_len != 1)
    return 1;

  if (a_len != 1 && b_len == 1)
    return -1;

  if (a_len == 1 && b_len == 1)
    {
      int rank_a = single_letter_subset_rank (a[0]);
      int rank_b = single_letter_subset_rank (b[0]);

      if (rank_a < rank_b)
	return 1;
      else
	return -1;
    }
  else
    {
      int rank_a = multi_letter_subset_rank(a);
      int rank_b = multi_letter_subset_rank(b);

      /* Using alphabetical/lexicographical order if they have same rank.  */
      if (rank_a == rank_b)
	/* The return value of strcmp has opposite meaning.  */
	return -strcmp (a.c_str (), b.c_str ());
      else
	return (rank_a < rank_b) ? 1 : -1;
    }
}

/* Add new subset to list.  */

void
riscv_subset_list::add (const char *subset, int major_version,
			int minor_version)
{
  riscv_subset_t *itr;
  if (strcmp (subset, "xv5") == 0)
    {
      major_version = 5;
      subset = "xandes";
    }

  riscv_subset_t *s = new riscv_subset_t ();

  if (m_head == NULL)
    m_head = s;

  s->name = subset;
  s->major_version = major_version;
  s->minor_version = minor_version;
  s->next = NULL;

  if (m_tail == NULL)
    {
      m_tail = s;
      return;
    }

  /* e, i or g should be first subext, never come here.  */
  gcc_assert (subset[0] != 'e'
	      && subset[0] != 'i'
	      && subset[0] != 'g');

  /* Add second element.  */
  if (m_tail == m_head)
    {
      gcc_assert (m_head->next == NULL);
      m_head->next = s;
      m_tail = s;
      return;
    }

  gcc_assert (m_head->next != NULL);

  /* Subset list must in canonical order, but implied subset won't
     add in canonical order.  */
  for (itr = m_head; itr->next != NULL; itr = itr->next)
    {
      riscv_subset_t *next = itr->next;
      int cmp = subset_cmp (s->name, next->name);
      gcc_assert (cmp != 0);

      if (cmp > 0)
	{
	  s->next = next;
	  itr->next = s;
	  return;
	}
    }

  /* Insert at tail of the list.  */
  itr->next = s;
  m_tail = s;
}

/* Find defined version from OPT and do naive add() */
void
riscv_subset_list::add (const char *subset, unsigned major_version,
			unsigned minor_version, const arch_options_t *opt)
{
  gcc_assert (opt);
  arch_options_default_version (opt, subset, &major_version, &minor_version);
  add (subset, major_version, minor_version);
}

/* Convert subset info to string with explicit version info,
   VERSION_P to determine append version info or not.  */

std::string
riscv_subset_list::to_string (bool version_p) const
{
  std::ostringstream oss;
  oss << "rv" << m_xlen;

  bool first = true;
  riscv_subset_t *subset = m_head;

  while (subset != NULL)
    {
      /* For !version_p, we only separate extension with underline for
	 multi-letter extension.  */
      if (!first &&
	  (version_p || subset->name.length() > 1))
	oss << '_';
      first = false;

      oss << subset->name;

      if (version_p)
	oss  << subset->major_version
	     << 'p'
	     << subset->minor_version;

      subset = subset->next;
    }

  return oss.str ();
}

/* Find subset in list with version checking, return NULL if not found.
   major/minor version checking can be ignored if major_version/minor_version
   is RISCV_DONT_CARE_VERSION.  */

riscv_subset_t *
riscv_subset_list::lookup (const char *subset, int major_version,
			   int minor_version) const
{
  riscv_subset_t *s;

  for (s = m_head; s != NULL; s = s->next)
    if (strcasecmp (s->name.c_str (), subset) == 0)
      {
	if ((major_version != RISCV_DONT_CARE_VERSION)
	    && (s->major_version != major_version))
	  return NULL;

	if ((minor_version != RISCV_DONT_CARE_VERSION)
	    && (s->minor_version != minor_version))
	  return NULL;

	return s;
      }

  return s;
}

/* Return string which contains all supported standard extensions in
   canonical order.  */

static const char *
riscv_supported_std_ext (void)
{
  return "mafdqlcbjtpvn";
}

static const char *
riscv_convert_nds_ext (const char *p)
{
  if (strncmp (p, "v5f", 3) == 0)
    return "imafcxandes";
  else if (strncmp (p, "v5d", 3) == 0)
    return "imafdcxandes";
  else if (strncmp (p, "v5", 2) == 0)
    return "imacxandes";
  else
    return p;
}

/* Parsing subset version.

   Return Value:
     Points to the end of version

   Arguments:
     `p`: Current parsing position.
     `major_version`: Parsing result of major version, using
      default_major_version if version is not present in arch string.
     `minor_version`: Parsing result of minor version, set to 0 if version is
     not present in arch string, but set to `default_minor_version` if
     `major_version` using default_major_version.
     `default_major_version`: Default major version.
     `default_minor_version`: Default minor version.
     `std_ext_p`: True if parsing std extension.  */

const char *
riscv_subset_list::parsing_subset_version (const char *p,
					   unsigned *major_version,
					   unsigned *minor_version,
					   unsigned default_major_version,
					   unsigned default_minor_version,
					   bool std_ext_p)
{
  bool major_p = true;
  unsigned version = 0;
  unsigned major = 0;
  unsigned minor = 0;
  char np;

  bool digit_start_p = ISDIGIT (*p);

  for (; *p; ++p)
    {
      if (digit_start_p && *p == 'p')
	{
	  np = *(p + 1);

	  if (!ISDIGIT (np))
	    {
	      /* Might be beginning of `p` extension.  */
	      if (std_ext_p)
		{
		  *major_version = version;
		  *minor_version = 0;
		  return p;
		}
	      else
		{
		  error_at (m_loc, "%<-march=%s%>: Expect number "
			    "after %<%dp%>.", m_arch, version);
		  return NULL;
		}
	    }

	  major = version;
	  major_p = false;
	  version = 0;
	}
      else if (ISDIGIT (*p))
	version = (version * 10) + (*p - '0');
      else
	break;
    }

  if (major_p)
    major = version;
  else
    minor = version;

  if (major == 0 && minor == 0)
    {
      /* We didn't find any version string, use default version.  */
      *major_version = default_major_version;
      *minor_version = default_minor_version;
    }
  else
    {
      *major_version = major;
      *minor_version = minor;
    }
  return p;
}

/* Parsing function for standard extensions.

   Return Value:
     Points to the end of extensions.

   Arguments:
     `p`: Current parsing position.  */

const char *
riscv_subset_list::parse_std_ext (const char *p)
{
  const char *all_std_exts = riscv_supported_std_ext ();
  const char *std_exts = all_std_exts;

  unsigned major_version = 0;
  unsigned minor_version = 0;
  char std_ext = '\0';

  /* First letter must start with i, e or g.  */
  switch (*p)
    {
    case 'i':
      p++;
      p = parsing_subset_version (p, &major_version, &minor_version,
				  /* default_major_version= */ 2,
				  /* default_minor_version= */ 0,
				  /* std_ext_p= */ true);
      add ("i", major_version, minor_version);
      break;

    case 'e':
      p++;
      p = parsing_subset_version (p, &major_version, &minor_version,
				  /* default_major_version= */ 1,
				  /* default_minor_version= */ 9,
				  /* std_ext_p= */ true);

      add ("e", major_version, minor_version);

      if (m_xlen > 32)
	{
	  error_at (m_loc, "%<-march=%s%>: rv%de is not a valid base ISA",
		    m_arch, m_xlen);
	  return NULL;
	}
      break;

    case 'g':
      p++;
      p = parsing_subset_version (p, &major_version, &minor_version,
				  /* default_major_version= */ 2,
				  /* default_minor_version= */ 0,
				  /* std_ext_p= */ true);
      add ("i", major_version, minor_version);

      for (; *std_exts != 'q'; std_exts++)
	{
	  const char subset[] = {*std_exts, '\0'};
	  /* Check that standard extension isn't disabled by option. */
	  if (!arch_options_disabled_p(&std_ext_options[0], subset))
	    add (subset, major_version, minor_version);
	}
      break;

    default:
      error_at (m_loc, "%<-march=%s%>: first ISA subset must be %<e%>, "
		"%<i%> or %<g%>", m_arch);
      return NULL;
    }

  while (*p)
    {
      char subset[2] = {0, 0};

      if (*p == 'x' || *p == 's' || *p == 'h' || *p == 'z')
	break;

      if (*p == '_')
	{
	  p++;
	  continue;
	}

      std_ext = *p;

      /* Checking canonical order.  */
      while (*std_exts && std_ext != *std_exts)
	{
	  subset[0] = *std_exts;

	  /* Check that standard extension is enabled by option. */
	  if (arch_options_enabled_p(&std_ext_options[0], subset))
	    add (subset, 2, 0, std_ext_options);
	  std_exts++;
	}

      if (std_ext != *std_exts)
	{
	  if (strchr (all_std_exts, std_ext) == NULL)
	    error_at (m_loc, "%<-march=%s%>: unsupported ISA subset %<%c%>",
		      m_arch, *p);
	  else
	    error_at (m_loc,
		      "%<-march=%s%>: ISA string is not in canonical order. "
		      "%<%c%>", m_arch, *p);
	  return NULL;
	}

      std_exts++;

      char nc = *++p;
      p = parsing_subset_version (p, &major_version, &minor_version,
				  /* default_major_version= */ 2,
				  /* default_minor_version= */ 0,
				  /* std_ext_p= */ true);

      subset[0] = std_ext;

      if (!ISDIGIT (nc))
	arch_options_default_version (std_ext_options, subset, &major_version,
				      &minor_version);

      /* Check that standard extension isn't disabled by option. */
      if (!arch_options_disabled_p(&std_ext_options[0], subset))
	add (subset, major_version, minor_version);
    }

  /* Check the reminding standard extension is enabled by option. */
  while (*std_exts)
    {
      char subset[2] = {*std_exts, 0};

      if (arch_options_enabled_p(&std_ext_options[0], subset))
	add (subset, 2, 0, std_ext_options);
      std_exts++;
    }
  return p;
}


/* Check any implied extensions for EXT.  */
void
riscv_subset_list::handle_implied_ext (riscv_subset_t *ext)
{
  riscv_implied_info_t *implied_info;
  for (implied_info = &riscv_implied_info[0];
       implied_info->ext;
       ++implied_info)
    {
      if (strcmp (ext->name.c_str (), implied_info->ext) != 0)
	continue;

      /* Skip if implied extension already present.  */
      if (lookup (implied_info->implied_ext))
	continue;

      add (implied_info->implied_ext, implied_info->ext);
    }
}

/* Add new subset to list, but using default version based on the following priority.
 * 1. subset's predefined default value in different option table.
 * 2. ext's predefined default value in different option table.
 * 3. Use ending entry's default value in according option table.
 */

void
riscv_subset_list::add (const char *subset, const char *ext)
{
  unsigned int major_version = 0, minor_version = 0;
  arch_options_t *opt;
  int i;

  /* Try to find predefined default version for subset. */
  for (i = 0; i < NUM_EXTS_KIND; ++i)
    {
      opt = ext_options[i];
      arch_options_default_version (opt, subset, &major_version,
				    &minor_version);

      if (major_version != 0 || minor_version != 0)
	{
	  add (subset, major_version, minor_version);
	  return;
	}
    }

  /* Try to find predefined default version for ext. */
  for (i = 0; i < NUM_EXTS_KIND; ++i)
    {
      opt = ext_options[i];
      arch_options_default_version (opt, ext, &major_version,
				    &minor_version);

      if (major_version != 0 || minor_version != 0)
	{
	  add (subset, major_version, minor_version);
	  return;
	}
    }

  if (subset[0] == 'z')
    opt = nonstd_z_ext_options;
  else if (subset[0] == 'x')
    opt = nonstd_x_ext_options;
  else
    opt = std_ext_options;

  /* Get the ending entry of related option table. */
  for (; !arch_options_end_p(opt); ++opt)
    ;
  add (subset, opt->default_major_version, opt->default_minor_version);
}

/* Parsing function for multi-letter extensions.

   Return Value:
     Points to the end of extensions.

   Arguments:
     `p`: Current parsing position.
     `ext_type`: What kind of extensions, 's', 'h', 'z' or 'x'.
     `ext_type_str`: Full name for kind of extension.  */

const char *
riscv_subset_list::parse_multiletter_ext (const char *p,
					  const char *ext_type,
					  const char *ext_type_str,
					  const arch_options_t *opt)
{
  unsigned major_version = 0;
  unsigned minor_version = 0;
  size_t ext_type_len = strlen (ext_type);

  while (*p)
    {
      if (*p == '_')
	{
	  p++;
	  continue;
	}

      if (strncmp (p, ext_type, ext_type_len) != 0)
	break;

      char *subset = xstrdup (p);
      char *q = subset;
      const char *end_of_version;

      if (strncmp(subset, "xv5", 3) == 0)
	q += 2;

      while (*++q != '\0' && *q != '_' && !ISDIGIT (*q))
	;

      end_of_version
	= parsing_subset_version (q, &major_version, &minor_version,
				  /* default_major_version= */ 2,
				  /* default_minor_version= */ 0,
				  /* std_ext_p= */ FALSE);

      *q = '\0';

      if (strlen (subset) == 1)
	{
	  error_at (m_loc, "%<-march=%s%>: name of %s must be more than 1 letter",
		    m_arch, ext_type_str);
	  free (subset);
	  return NULL;
	}

      /* Update version to default one if it doesn't be specified. */
      if (opt && (q == end_of_version))
	arch_options_default_version(opt, subset, &major_version, &minor_version);

      /* Check that non-standard-extension isn't disabled by option. */
      if (!opt)
	add (subset, major_version, minor_version);
      else if (!arch_options_disabled_p(opt, subset))
	add (subset, major_version, minor_version);
      free (subset);
      p += end_of_version - subset;

      if (*p != '\0' && *p != '_')
	{
	  error_at (m_loc, "%<-march=%s%>: %s must separate with _",
		    m_arch, ext_type_str);
	  return NULL;
	}
    }

  /* Check that the other non-standard-extension is enabled by option. */
  if (opt)
    {
      for (; !arch_options_end_p(opt); ++opt)
	{
	  if (!lookup(opt->ext) && opt->is_spec && opt->val)
	    add (opt->ext, opt->default_major_version, opt->default_minor_version);
    
          // Workaround: Implictly enable zfh if (v && (f || d)) is enabled.
          if (strcmp (opt->ext, "zfh") == 0 && !lookup(opt->ext) && lookup("v") &&
              (lookup("f") || lookup("d")))
            add (opt->ext, opt->default_major_version, opt->default_minor_version);
	}
    }

  return p;
}

/* Parsing arch string to subset list, return NULL if parsing failed.  */

riscv_subset_list *
riscv_subset_list::parse (const char *arch, location_t loc)
{
  riscv_subset_list *subset_list = new riscv_subset_list (arch, loc);
  riscv_subset_t *itr;
  const char *p = arch;
  if (strncmp (p, "rv32", 4) == 0)
    {
      subset_list->m_xlen = 32;
      p += 4;
    }
  else if (strncmp (p, "rv64", 4) == 0)
    {
      subset_list->m_xlen = 64;
      p += 4;
    }
  else
    {
      error_at (loc, "%<-march=%s%>: ISA string must begin with rv32 or rv64",
		arch);
      goto fail;
    }

  /* Convert nds string to standard extension.  */
  p = riscv_convert_nds_ext (p);

  /* Parsing standard extension.  */
  p = subset_list->parse_std_ext (p);

  if (p == NULL)
    goto fail;

  /* Parsing supervisor extension.  */
  p = subset_list->parse_multiletter_ext (p, "s", "supervisor extension");

  if (p == NULL)
    goto fail;

  /* Parsing hypervisor extension.  */
  p = subset_list->parse_multiletter_ext (p, "h", "hypervisor extension");

  if (p == NULL)
    goto fail;

  /* Parsing sub-extensions.  */
  p = subset_list->parse_multiletter_ext (p, "z", "sub-extension",
					  nonstd_z_ext_options);
  if (p == NULL)
    goto fail;

  /* Parsing non-standard extension.  */
  p = subset_list->parse_multiletter_ext (p, "x", "non-standard extension",
					  nonstd_x_ext_options);

  if (p == NULL)
    goto fail;

  if (*p != '\0')
    {
      error_at (loc, "%<-march=%s%>: unexpected ISA string at end: %qs",
               arch, p);
      goto fail;
    }

  for (itr = subset_list->m_head; itr != NULL; itr = itr->next)
    {
      subset_list->handle_implied_ext (itr);
    }

  return subset_list;

fail:
  delete subset_list;
  return NULL;
}

/* Return the current arch string.  */

std::string
riscv_arch_str (bool version_p)
{
  gcc_assert (current_subset_list);
  return current_subset_list->to_string (version_p);
}

/* Type for pointer to member of gcc_options.  */
typedef int (gcc_options::*opt_var_ref_t);

/* Types for recording extension to internal flag.  */
struct riscv_ext_flag_table_t {
  const char *ext;
  opt_var_ref_t var_ref;
  int mask;
};

/* Mapping table between extension to internal flag.  */
static const riscv_ext_flag_table_t riscv_ext_flag_table[] =
{
  {"e", &gcc_options::x_target_flags, MASK_RVE},
  {"m", &gcc_options::x_target_flags, MASK_MUL},
  {"a", &gcc_options::x_target_flags, MASK_ATOMIC},
  {"f", &gcc_options::x_target_flags, MASK_HARD_FLOAT},
  {"d", &gcc_options::x_target_flags, MASK_DOUBLE_FLOAT},
  {"c", &gcc_options::x_target_flags, MASK_RVC},

  {"zba",    &gcc_options::x_riscv_zb_subext, MASK_ZBA},
  {"zbb",    &gcc_options::x_riscv_zb_subext, MASK_ZBB},
  {"zbc",    &gcc_options::x_riscv_zb_subext, MASK_ZBC},
  {"zbs",    &gcc_options::x_riscv_zb_subext, MASK_ZBS},
  {NULL, NULL, 0}
};

/* Parse a RISC-V ISA string into an option mask.  Must clear or set all arch
   dependent mask bits, in case more than one -march string is passed.  */

static void
riscv_parse_arch_string (const char *isa,
			 struct gcc_options *opts,
			 location_t loc)
{
  riscv_subset_list *subset_list;
  subset_list = riscv_subset_list::parse (isa, loc);
  int *flags = &opts->x_target_flags;
  if (!subset_list)
    return;

  if (opts)
    {
      const riscv_ext_flag_table_t *arch_ext_flag_tab;
      /* Clean up target flags before we set.  */
      for (arch_ext_flag_tab = &riscv_ext_flag_table[0];
	   arch_ext_flag_tab->ext;
	   ++arch_ext_flag_tab)
	opts->*arch_ext_flag_tab->var_ref &= ~arch_ext_flag_tab->mask;

      if (subset_list->xlen () == 32)
	opts->x_target_flags &= ~MASK_64BIT;
      else if (subset_list->xlen () == 64)
	opts->x_target_flags |= MASK_64BIT;


      for (arch_ext_flag_tab = &riscv_ext_flag_table[0];
	   arch_ext_flag_tab->ext;
	   ++arch_ext_flag_tab)
	{
	  if (subset_list->lookup (arch_ext_flag_tab->ext))
	    opts->*arch_ext_flag_tab->var_ref |= arch_ext_flag_tab->mask;
	}
    }

  *flags &= ~MASK_RVV;
  *flags &= ~MASK_ZFH;
  if (subset_list->lookup ("v"))
    {
      *flags |= MASK_RVV;
      if (subset_list->lookup ("f") || subset_list->lookup ("d"))
        *flags |= MASK_ZFH;
    }

  if (subset_list->lookup ("zfh"))
    *flags |= MASK_ZFH;

  *flags &= ~MASK_DSP;
  if (subset_list->lookup ("p"))
    *flags |= MASK_DSP;

  // initialize v5 related mask
  if ((target_flags_explicit & MASK_V5) == 0)
    *flags &= ~MASK_V5;
  if ((target_flags_explicit & MASK_BFO) == 0)
    *flags &= ~MASK_BFO;
  if ((target_flags_explicit & MASK_BBCS) == 0)
    *flags &= ~MASK_BBCS;
  if ((target_flags_explicit & MASK_BIMM) == 0)
    *flags &= ~MASK_BIMM;
  if ((target_flags_explicit & MASK_LEA) == 0)
    *flags &= ~MASK_LEA;
  if ((target_flags_explicit & MASK_FP16) == 0)
    *flags &= ~MASK_FP16;

  if (subset_list->lookup ("xandes"))
    {
      if ((target_flags_explicit & MASK_V5) == 0)
	*flags |= MASK_V5;
      if ((target_flags_explicit & MASK_BFO) == 0)
	*flags |= MASK_BFO;
      if ((target_flags_explicit & MASK_BBCS) == 0)
	*flags |= MASK_BBCS;
      if ((target_flags_explicit & MASK_BIMM) == 0)
	*flags |= MASK_BIMM;
      if ((target_flags_explicit & MASK_LEA) == 0)
	*flags |= MASK_LEA;

      if (subset_list->lookup ("xefhw"))
	if ((target_flags_explicit & MASK_FP16) == 0)
	  *flags |= MASK_FP16;
    }

  if (current_subset_list)
    delete current_subset_list;

  current_subset_list = subset_list;
}

static void
riscv_parse_cpu_string (const char *cpu, int *flags)
{
  // 45-series cpuss have CALU
  if ((target_flags_explicit & MASK_CMOV) == 0)
    {
      *flags &= ~MASK_CMOV;
      if (strcmp (cpu, "n45") == 0 || strcmp (cpu, "nx45") == 0
	  || strcmp (cpu, "n45f") == 0 || strcmp (cpu, "nx45f") == 0
	  || strcmp (cpu, "d45") == 0 || strcmp (cpu, "d45f") == 0
	  || strcmp (cpu, "a45") == 0 || strcmp (cpu, "ax45") == 0)
	*flags |= MASK_CMOV;
    }
}

/* Implement TARGET_HANDLE_OPTION.  */

static bool
riscv_handle_option (struct gcc_options *opts,
		     struct gcc_options *opts_set ATTRIBUTE_UNUSED,
		     const struct cl_decoded_option *decoded,
		     location_t loc)
{
  switch (decoded->opt_index)
    {
    case OPT_march_:
      riscv_parse_arch_string (decoded->arg, opts, loc);
      return true;

    case OPT_mtune_:
      riscv_parse_cpu_string (decoded->arg, &opts->x_target_flags);
      return true;

    default:
      return true;
    }
}

/* Implement TARGET_OPTION_OPTIMIZATION_TABLE.  */
static const struct default_options riscv_option_optimization_table[] =
  {
    { OPT_LEVELS_1_PLUS, OPT_fsection_anchors, NULL, 1 },
    { OPT_LEVELS_2_PLUS, OPT_free, NULL, 1 },
    { OPT_LEVELS_SIZE, OPT_msave_restore, NULL, 1 },
#if TARGET_LINUX_ABI == 0
    /* Disable -fdelete-null-pointer-checks by default in ELF toolchain.  */
    { OPT_LEVELS_ALL, OPT_fdelete_null_pointer_checks, NULL, 0 },
#endif
    { OPT_LEVELS_3_PLUS, OPT_mipa_escape_analysis, NULL, 1 },
    { OPT_LEVELS_NONE, 0, NULL, 0 }
  };

#undef TARGET_OPTION_OPTIMIZATION_TABLE
#define TARGET_OPTION_OPTIMIZATION_TABLE riscv_option_optimization_table

#undef TARGET_HANDLE_OPTION
#define TARGET_HANDLE_OPTION riscv_handle_option

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;


/* Traverse all input arch options and set its value */

void parse_arch_options(const char *option)
{
  bool val = true;
  if (strncmp(option, "mno-", 4) == 0)
    {
      val = false;
      option += 4;
    }
  else
      option += 1;

  int i;
  for (i = 0; i < NUM_EXTS_KIND; ++i)
    {
      arch_options_t *opt = ext_options[i];
      for (; !arch_options_end_p(opt); ++opt)
	if (strncmp(opt->option_name, option, strlen(opt->option_name)) == 0)
	  {
	    opt->is_spec = true;
            opt->val = val;
	  }
    }
}

/* Rewrite march with other arch options(e.g. -matomic, -mext-dsp, ...) */

const char *
riscv_rewrite_march(int argc, const char **argv)
{
  int i;
  riscv_subset_list *subset_list;

  for (i = 0; i < argc; ++i)
    {
      parse_arch_options(argv[i]);
    }

  for (i = argc - 1; i >= 0; --i)
    {
      if (strncmp(argv[i], "march=", 6) == 0)
	{
	  subset_list = riscv_subset_list::parse (argv[i] + 6, location_t());
	  break;
	}
    }

  gcc_assert(subset_list->to_string(true).length() < MAX_ARCH_STRING_LEN);
  strncpy (riscv_arch_string, subset_list->to_string(true).c_str(),
	   MAX_ARCH_STRING_LEN);
  delete subset_list;
  return &riscv_arch_string[0];
}
