#include "ruby.h"
#include <ruby/encoding.h>

#include "libMultiMarkdown.h"
#include "mmd.h"
#include "d_string.h"

// Tries to convert a C string into an encoded ruby String
static VALUE encoded_str_new2(char *str, char *encoding) {
  VALUE result = rb_str_new2(str);

  int enc = rb_enc_find_index(encoding);
  if (enc > 0) {
    rb_enc_associate_index(result, enc);
  }

  return result;
}

static VALUE rb_cMultiMarkdown;

static int get_exts(VALUE self) {
  int extensions = 0;
  if (rb_funcall(self, rb_intern("complete"), 0) == Qtrue)
    extensions = extensions | EXT_COMPLETE;
  if (rb_funcall(self, rb_intern("snippet"), 0) == Qtrue)
    extensions = extensions | EXT_SNIPPET;
  if (rb_funcall(self, rb_intern("no_smart_quotes"), 0) != Qtrue)
    extensions = extensions | EXT_SMART;
  if (rb_funcall(self, rb_intern("no_footnotes"), 0) != Qtrue)
    extensions = extensions | EXT_NOTES;
  if (rb_funcall(self, rb_intern("no_anchors"), 0) == Qtrue)
    extensions = extensions | EXT_NO_LABELS;
  //if (rb_funcall(self, rb_intern("filter_styles"), 0) == Qtrue)
  //  extensions = extensions | EXT_FILTER_STYLES;
  //if (rb_funcall(self, rb_intern("filter_html"), 0) == Qtrue)
  //  extensions = extensions | EXT_FILTER_HTML;
  if (rb_funcall(self, rb_intern("process_html"), 0) == Qtrue)
    extensions = extensions | EXT_PROCESS_HTML;
  if (rb_funcall(self, rb_intern("no_metadata"), 0) == Qtrue)
    extensions = extensions | EXT_NO_METADATA;
  //if (rb_funcall(self, rb_intern("obfuscate_email_addresses"), 0) == Qtrue)
  //  extensions = extensions | EXT_OBFUSCATE;
  if (rb_funcall(self, rb_intern("critic_markup_accept_all"), 0) == Qtrue)
    extensions = extensions | EXT_CRITIC | EXT_CRITIC_ACCEPT;
  if (rb_funcall(self, rb_intern("critic_markup_reject_all"), 0) == Qtrue)
    extensions = extensions | EXT_CRITIC | EXT_CRITIC_REJECT;
  if (rb_funcall(self, rb_intern("random_footnote_anchor_numbers"), 0) == Qtrue)
    extensions = extensions | EXT_RANDOM_FOOT;
  //if (rb_funcall(self, rb_intern("escaped_line_breaks"), 0) == Qtrue)
  //  extensions = extensions | EXT_ESCAPED_LINE_BREAKS;

  /* Compatibility overwrites all other extensions */
  if (rb_funcall(self, rb_intern("compatibility"), 0) == Qtrue)
    extensions = EXT_COMPATIBILITY;
  return extensions;
}

typedef struct engine_manager {
  mmd_engine *mmd_engine;
} engine_manager;

mmd_engine *get_mmd_engine(VALUE self) {
  engine_manager *manager;
  Data_Get_Struct(self, engine_manager, manager);

  return manager->mmd_engine;
}

static void free_engine_manager(engine_manager* manager) {
  if (manager->mmd_engine) {
    mmd_engine_free(manager->mmd_engine, true);
  }
  free(manager);
}

static VALUE rb_multimarkdown_allocate(VALUE class) {
  engine_manager *manager = malloc(sizeof(engine_manager));
  manager->mmd_engine = NULL;

  return Data_Wrap_Struct(class, NULL, free_engine_manager, manager);
}

static VALUE rb_multimarkdown_start_engine(VALUE self, VALUE text) {
  Check_Type(text, T_STRING);

  engine_manager *manager;
  Data_Get_Struct(self, engine_manager, manager);

  manager->mmd_engine = mmd_engine_create_with_string(StringValuePtr(text), get_exts(self));

  return self;
}

static VALUE rb_multimarkdown_to_html(VALUE self) {
  char *html = mmd_engine_convert(get_mmd_engine(self), FORMAT_HTML);
  VALUE result = encoded_str_new2(html, "UTF-8");
  free(html);

  return result;
}

static VALUE rb_multimarkdown_to_latex(VALUE self) {
  char *latex = mmd_engine_convert(get_mmd_engine(self), FORMAT_LATEX);
  VALUE result = encoded_str_new2(latex, "UTF-8");
  free(latex);

  return result;
}

static VALUE rb_multimarkdown_extract_metadata_keys(VALUE self) {
  char *metadata_keys = mmd_engine_metadata_keys(get_mmd_engine(self));
  VALUE str = encoded_str_new2(metadata_keys, "UTF-8");
  free(metadata_keys);

  return rb_funcall(str, rb_intern("split"), 1, rb_str_new2("\n"));
}

static VALUE rb_multimarkdown_extract_metadata_value(VALUE self, VALUE key) {
  Check_Type(key, T_STRING);
  char *pkey = StringValuePtr(key);

  char *metadata = mmd_engine_metavalue_for_key(get_mmd_engine(self), pkey);
  VALUE result = encoded_str_new2(metadata, "UTF-8");

  return result;
}

void Init_multi_markdown() {

  rb_cMultiMarkdown = rb_define_class("MultiMarkdown", rb_cObject);

  rb_define_alloc_func(rb_cMultiMarkdown, rb_multimarkdown_allocate);
  rb_define_protected_method(rb_cMultiMarkdown, "start_engine", rb_multimarkdown_start_engine, 1);

  /* Document-method: MultiMarkdown#to_html
   *
   * Return string containing HTML generated from MultiMarkdown text
   */
  rb_define_method(rb_cMultiMarkdown, "to_html", rb_multimarkdown_to_html, 0);

  /* Document-method: MultiMarkdown#to_latex
   *
   * Return string containing latex generated from MultiMarkdown text
   */
  rb_define_method(rb_cMultiMarkdown, "to_latex", rb_multimarkdown_to_latex, 0);

  /* Document-method: MultiMarkdown#extract_metadata_keys
   *
   * Return Array of metadata keys
   */
  rb_define_method(rb_cMultiMarkdown, "extract_metadata_keys", rb_multimarkdown_extract_metadata_keys, 0);

  /* Document-method: MultiMarkdown#extract_metadata_value
   * :call-seq: extract_metadata_value(key)
   *
   * Fetches metadata specified by +key+ from MultiMarkdown text
   */
  rb_define_method(rb_cMultiMarkdown, "extract_metadata_value", rb_multimarkdown_extract_metadata_value, 1);

  /* Document-const: MultiMarkdown::MMD_VERSION
   *
   * The version of the MultiMarkdown-6 library
   */
  char *version = mmd_version();
  rb_define_const(rb_cMultiMarkdown, "MMD_VERSION", rb_str_new2(version));
  free(version);

}
