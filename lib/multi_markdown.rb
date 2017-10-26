require 'multi_markdown.so'
require 'multi_markdown/version'

# Front-end to fletcher penney's implementation of MultiMarkdown
#
# A simple processor:
#     >> puts MultiMarkdown.new("Hello, World.").to_html
#     <p>Hello, World.</p>
#
# With other stuff:
#     >> puts MultiMarkdown.new("_Hello World!_", :smart, :filter_html).to_html
#     <p><em>Hello World!</em></p>
#
class MultiMarkdown

  EXTENSIONS = {
    "compatibility" => {:desc => "Markdown compatibility mode (disables all other options)", :short => "c"},
    "complete" => {:desc => "Force complete document", :short => "f"},
    "snippet" => {:desc => "Force snippet only", :short => "s"},
    "no_smart_quotes" => {:desc => "Disable Smart quotes", :short => false},
    "no_footnotes" => {:desc => "Disable Footnotes", :short => false},
    "no_anchors" => {:desc => "Don't add anchors to headers, etc.", :short => false},
    "filter_styles" => {:desc => "Filter out style blocks", :short => false},
    "filter_html" => {:desc => "Filter out raw HTML", :short => false},
    "process_html" => {:desc => "Process Markdown inside HTML", :short => false},
    "no_metadata" => {:desc => "Don't parse Metadata", :short => false},
    "obfuscate_email_addresses" => {:desc => "Mask email addresses", :short => false},
    "critic_markup_accept_all" => {:desc => "CriticMarkup: Accept all proposed changes", :short => "a"},
    "critic_markup_reject_all" => {:desc => "CriticMarkup: Reject all proposed changes", :short => "r"},
    "random_footnote_anchor_numbers" => {:desc => "Use random numbers for footnote link anchors", :short => false},
    "escaped_line_breaks" => {:desc => "Escaped line break", :short => false}
  }

  EXTENSIONS.keys.each do |ext|
    attr_accessor ext
  end

  # Create a new MultiMarkdown processor. The `text` argument is a string
  # containing MultiMarkdown text. Variable other arguments may be supplied to
  # set various processing options. See MultiMarkdown::EXTENSIONS for more.
  def initialize(text, *extensions)
    extensions.each do |ext|
      raise "Unknown extension: #{ext.inspect}" unless EXTENSIONS.keys.include?(ext.to_s)
      send("#{ext}=", true)
    end
    self.start_engine(text);
  end

  alias extract_metadata extract_metadata_value

  # Returns a Hash cointaining all Metadata
  #
  #
  def metadata(key = nil)
    if @cached_metadata.nil?
      @cached_metadata = {}
      extract_metadata_keys.each do |k|
        @cached_metadata[k.downcase] = extract_metadata_value(k)
      end
    end

    if key
      @cached_metadata[key.to_s.downcase]
    else
      @cached_metadata.dup
    end
  end

end
