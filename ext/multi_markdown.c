#include "ruby.h"
#include "../MultiMarkdown-4/parser.h"

static VALUE rb_cMultiMarkdown;

int get_exts(VALUE self) {
  int extensions = 0;
  if (rb_funcall(self, rb_intern("smart"), 0) == Qtrue)
    extensions = extensions | EXT_SMART;
  if (rb_funcall(self, rb_intern("notes"), 0) == Qtrue)
    extensions = extensions | EXT_NOTES;
  if (rb_funcall(self, rb_intern("filter_html"), 0) == Qtrue)
    extensions = extensions | EXT_FILTER_HTML;
  if (rb_funcall(self, rb_intern("filter_styles"), 0) == Qtrue)
    extensions = extensions | EXT_FILTER_STYLES;
  if (rb_funcall(self, rb_intern("process_html"), 0) == Qtrue)
    extensions = extensions | EXT_PROCESS_HTML;
  /* Compatibility overwrites all other extensions */
  if (rb_funcall(self, rb_intern("compatibility"), 0) == Qtrue)
    extensions = EXT_COMPATIBILITY;
  return extensions;
}

char *get_text(VALUE self) {
  /* grab char pointer to multimarkdown input text */
  VALUE text = rb_iv_get(self, "@text");
  Check_Type(text, T_STRING);
  return StringValuePtr(text);
}

static VALUE rb_multimarkdown_to_html(VALUE self) {
  char *html = markdown_to_string(get_text(self), get_exts(self), HTML_FORMAT);
  VALUE result = rb_str_new2(html);
  free(html);

  return result;
}

static VALUE rb_multimarkdown_to_latex(VALUE self) {
  char *latex = markdown_to_string(get_text(self), get_exts(self), LATEX_FORMAT);
  VALUE result = rb_str_new2(latex);
  free(latex);

  return result;
}

static VALUE rb_multimarkdown_extract_metadata_keys(VALUE self) {
  char *metadata_keys = extract_metadata_keys(get_text(self), get_exts(self));
  VALUE str = rb_str_new2(metadata_keys);
  free(metadata_keys);

  return rb_funcall(str, rb_intern("split"), 1, rb_str_new2("\n"));
}

static VALUE rb_multimarkdown_extract_metadata_value(VALUE self, VALUE key) {
  Check_Type(key, T_STRING);
  char *pkey = StringValuePtr(key);

  char *metadata = extract_metadata_value(get_text(self), get_exts(self), pkey);
  VALUE result = rb_str_new2(metadata);
  free(metadata);

  return result;
}

void Init_multi_markdown() {

  rb_cMultiMarkdown = rb_define_class("MultiMarkdown", rb_cObject);

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

  rb_define_const(rb_cMultiMarkdown, "MMD_VERSION", rb_str_new2(MMD_VERSION));
  /* Document-const: MultiMarkdown::MMD_VERSION
   *
   * The version of the MultiMarkdown-4 library
   */
}
