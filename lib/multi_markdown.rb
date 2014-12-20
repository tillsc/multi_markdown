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

  # Set `true` to have smarty-like quote translation performed.
  attr_accessor :smart

  # Set `true` to only emit a partial document and not a full one.
  attr_accessor :snippet

  # Set `true` to have footnotes processed.
  attr_accessor :notes

  # Do not output `<style>` tags included in the source text.
  attr_accessor :filter_styles

  # Do not output any raw HTML included in the source text.
  attr_accessor :filter_html

  # Process MultiMarkdown inside of raw HTML
  attr_accessor :process_html

  # Markdown compatibility mode
  attr_accessor :compatibility

  # Included for compatibility with RedCloth's interface.
  attr_accessor :fold_lines

  # Create a new MultiMarkdown processor. The `text` argument is a string
  # containing MultiMarkdown text. Variable other arguments may be supplied to
  # set various processing options:
  #
  # * `:smart` - Enable SmartyPants processing.
  # * `:snippet` - Only generate a partial document.
  # * `:notes` - Enable footnotes.
  # * `:filter_styles` - Do not output `<style>` tags included in the
  #   source text.
  # * `:filter_html` - Do not output raw HTML included in the
  #   source text.
  # * `:process_html` - Process MultiMarkdown code inside HTML tags.
  # * `:compatibility` - Process MultiMarkdown code in Markdown
  #   compatibility mode (disables all other extensions)
  # * `:fold_lines` - RedCloth compatible line folding (not used).
  #
  def initialize(text, *extensions)
    @text = text
    @smart = true
    @snippet = false
    @notes = true
    @filter_styles = false
    @filter_html = false
    @process_html = false
    @compatibility = false
    extensions.each { |e| send("#{e}=", true) }
    if @compatibility
      @smart = false
      @notes = false
      @process_html = false
    end
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
