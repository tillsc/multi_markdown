#include "ruby.h"
#include <ruby/encoding.h>

#include "libMultiMarkdown.h"
#include "mmd.h"
#include "d_string.h"

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
  if (rb_funcall(self, rb_intern("process_html"), 0) == Qtrue)
    extensions = extensions | EXT_PROCESS_HTML;
  if (rb_funcall(self, rb_intern("no_metadata"), 0) == Qtrue)
    extensions = extensions | EXT_NO_METADATA;
  if (rb_funcall(self, rb_intern("obfuscate_email_addresses"), 0) == Qtrue)
    extensions = extensions | EXT_OBFUSCATE;
  if (rb_funcall(self, rb_intern("critic_markup_accept_all"), 0) == Qtrue)
    extensions = extensions | EXT_CRITIC | EXT_CRITIC_ACCEPT;
  if (rb_funcall(self, rb_intern("critic_markup_reject_all"), 0) == Qtrue)
    extensions = extensions | EXT_CRITIC | EXT_CRITIC_REJECT;
  if (rb_funcall(self, rb_intern("random_footnote_anchor_numbers"), 0) == Qtrue)
    extensions = extensions | EXT_RANDOM_FOOT;

  /* Compatibility overwrites all other extensions */
  if (rb_funcall(self, rb_intern("compatibility"), 0) == Qtrue)
    extensions = EXT_COMPATIBILITY;
  return extensions;
}

// *** MMD Engine management

typedef struct engine_manager {
  mmd_engine *mmd_engine;
} engine_manager;

static void engine_manager_free(void *ptr) {
  engine_manager *manager = (engine_manager *)ptr;
  if (manager->mmd_engine) {
    mmd_engine_free(manager->mmd_engine, true);
    manager->mmd_engine = NULL;
  }
  xfree(manager);
}

static size_t engine_manager_memsize(const void *ptr) {
  return sizeof(engine_manager);
}

static const rb_data_type_t engine_manager_type = {
  "MultiMarkdown/engine_manager",
  { 0, engine_manager_free, engine_manager_memsize, },
  0, 0,
  RUBY_TYPED_FREE_IMMEDIATELY
};

static VALUE rb_multimarkdown_allocate(VALUE klass) {
  engine_manager *manager;
  VALUE obj = TypedData_Make_Struct(klass, engine_manager, &engine_manager_type, manager);
  manager->mmd_engine = NULL;
  return obj;
}

static engine_manager *get_manager(VALUE self) {
  engine_manager *manager;
  TypedData_Get_Struct(self, engine_manager, &engine_manager_type, manager);
  return manager;
}

static mmd_engine *get_mmd_engine(VALUE self) {
  return get_manager(self)->mmd_engine;
}

static VALUE rb_multimarkdown_start_engine(VALUE self, VALUE text) {
  Check_Type(text, T_STRING);

  engine_manager *manager = get_manager(self);

  if (manager->mmd_engine) {
    mmd_engine_free(manager->mmd_engine, true);
    manager->mmd_engine = NULL;
  }

  manager->mmd_engine = mmd_engine_create_with_string(StringValueCStr(text), get_exts(self));

  return self;
}

//*** Public Methods

static VALUE rb_multimarkdown_set_language(VALUE self, VALUE language) {
  short lang = ENGLISH;

  VALUE language_s = rb_obj_as_string(language);
  const char *s = StringValueCStr(language_s);

  if (strcmp(s, "de") == 0 || strcmp(s, "german") == 0) {
    lang = GERMANGUILL;
  } else if (strcmp(s, "ch") == 0 || strcmp(s, "de-ch") == 0 || strcmp(s, "swiss") == 0) {
    lang = GERMAN;
  } else if (strcmp(s, "nl") == 0 || strcmp(s, "dutch") == 0) {
    lang = DUTCH;
  } else if (strcmp(s, "fr") == 0 || strcmp(s, "french") == 0) {
    lang = FRENCH;
  } else if (strcmp(s, "sp") == 0 || strcmp(s, "spanish") == 0) {
    lang = SPANISH;
  } else if (strcmp(s, "sv") == 0 || strcmp(s, "swedish") == 0) {
    lang = SWEDISH;
  }

  mmd_engine_set_language(get_mmd_engine(self), lang);
  return language;
}

static VALUE rb_multimarkdown_to_html(VALUE self) {
  char *html = mmd_engine_convert(get_mmd_engine(self), FORMAT_HTML);
  VALUE result = rb_utf8_str_new_cstr(html);
  free(html);

  return result;
}

static VALUE rb_multimarkdown_to_latex(VALUE self) {
  char *latex = mmd_engine_convert(get_mmd_engine(self), FORMAT_LATEX);
  VALUE result = rb_utf8_str_new_cstr(latex);
  free(latex);

  return result;
}

static VALUE rb_multimarkdown_extract_metadata_keys(VALUE self) {
  mmd_engine *engine = get_mmd_engine(self);
  size_t end = 0;

  if (!mmd_engine_has_metadata(engine, &end)) {
    return rb_ary_new();
  }

  char *metadata_keys = mmd_engine_metadata_keys(engine);
  VALUE str = rb_utf8_str_new_cstr(metadata_keys);
  free(metadata_keys);

  return rb_funcall(str, rb_intern("split"), 1, rb_str_new_cstr("\n"));
}

static VALUE rb_multimarkdown_extract_metadata_value(VALUE self, VALUE key) {
  Check_Type(key, T_STRING);

  mmd_engine *engine = get_mmd_engine(self);
  size_t end = 0;

  if (!mmd_engine_has_metadata(engine, &end)) {
    return Qnil;
  }

  char *metadata = mmd_engine_metavalue_for_key(engine, StringValueCStr(key));
  if (metadata == NULL) {
    return Qnil;
  }

  return rb_utf8_str_new_cstr(metadata);
}

//*** Define Class

void Init_multi_markdown() {

  rb_cMultiMarkdown = rb_define_class("MultiMarkdown", rb_cObject);

  rb_define_alloc_func(rb_cMultiMarkdown, rb_multimarkdown_allocate);
  rb_define_protected_method(rb_cMultiMarkdown, "start_engine", rb_multimarkdown_start_engine, 1);


  //
  rb_define_method(rb_cMultiMarkdown, "language=", rb_multimarkdown_set_language, 1);

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
