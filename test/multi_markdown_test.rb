$: << File.join(File.dirname(__FILE__), "../lib")

require 'test/unit'
require 'multi_markdown'

class MultiMarkdownTest < Test::Unit::TestCase

  def test_extension_methods_present_on_multimarkdown_class
    assert MultiMarkdown.instance_methods.include?(:to_html),
        "MultiMarkdown class should respond to #to_html"
  end

  def test_simple_one_liner_to_html
    multimarkdown = MultiMarkdown.new('Hello World.')
    assert_respond_to multimarkdown, :to_html
    assert_equal "<p>Hello World.</p>", multimarkdown.to_html.strip
  end

  def test_inline_multimarkdown_to_html
    multimarkdown = MultiMarkdown.new('_Hello World_!')
    assert_respond_to multimarkdown, :to_html
    assert_equal "<p><em>Hello World</em>!</p>", multimarkdown.to_html.strip
  end

  def test_multimarkdown_in_html_to_html
    multimarkdown = MultiMarkdown.new('Hello <span>_World_</span>!',:process_html)
    assert_respond_to multimarkdown, :to_html
    assert_equal "<p>Hello <span><em>World</em></span>!</p>", multimarkdown.to_html.strip
  end

  def test_version_fits
    assert MultiMarkdown::VERSION =~ /^#{MultiMarkdown::MMD_VERSION}/,
        "Expected MultiMarkdown's version (#{MultiMarkdown::VERSION}) to start with the C library's version (#{MultiMarkdown::MMD_VERSION})"
  end

  def test_meta_attributes
    multimarkdown = MultiMarkdown.new(<<-eof)
meta1: Foo
meta2: Bar

Lorem Ipsum
    eof
    assert_equal ["meta1", "meta2"], multimarkdown.extract_metadata_keys()

    assert_equal "Foo", multimarkdown.extract_metadata_value("Meta1")
    assert_equal "Bar", multimarkdown.extract_metadata_value("Meta2")
  end

  def test_cached_metadata
      multimarkdown = MultiMarkdown.new(<<-eof)
MetaTheMeta1: Foo
MetaTheMeta2: Bar

Lorem Ipsum
    eof

    assert_equal({"metathemeta1" => "Foo", "metathemeta2" => "Bar"}, multimarkdown.metadata)

    assert_equal("Foo", multimarkdown.metadata("MetaTheMeta1"))
    assert_equal(nil, multimarkdown.metadata["MetaTheMeta1"])
  end


end
