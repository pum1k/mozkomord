#include "language.hpp"

template <>
void argp::KeywordOption<BF::MemoryType>::from_string(
    const std::vector<std::string_view> &strings)
{
    std::string_view data = strings[1];

    if (data == "static-safe")
        this->val = BF::MemoryType::static_safe;
    else if (data == "static-unsafe")
        this->val = BF::MemoryType::static_unsafe;
    else if (data == "static-loop")
        this->val = BF::MemoryType::static_loop;
    else if (data == "dynamic")
        this->val = BF::MemoryType::dynamic;
    else
    {
        throw std::invalid_argument("Could not convert \"" + std::string(data) +
                                    "\" to BF::MemoryType");
    }
}

template <>
void argp::KeywordOption<BF::MemDataType>::from_string(
    const std::vector<std::string_view> &strings)
{
    std::string_view data = strings[1];

    if (data == "8")
        this->val = BF::MemDataType::bit8;
    else if (data == "16")
        this->val = BF::MemDataType::bit16;
    else
    {
        throw options_error("Invalid value for \"--cell-size\" option.");
    }
}

void LangBF::process_options()
{
    // File
    // This option is queried later in the program
    if (!this->filename.is_set())
    {
        throw options_error("You must specify file to run.");
    }

    // Memory type
    this->mem_fact.set_mem_type(this->mem_type.get_val());

    // Memory size
    if (this->mem_size.get_val() > 0)
    {
        this->mem_fact.set_mem_size(this->mem_size.get_val());
    }
    else
    {
        throw options_error("Invalid value for \"--mem-size\" option.");
    }

    // Mem cell size
    this->mem_data_type = this->mem_cell_size.get_val();

    // Debug mode
    // No check required
    // This option is queried later in the program

    // Optimizations
    // This option is queried later in the program
    // if both debugging and optimizations are enabled, throw error
    if (this->debug.get_val() && this->optimize.get_val())
    {
        throw options_error(
            "Debugging and optimizations cannot be enabled both at the same "
            "time.");
    }
}

void LangBF::setup_program() { this->prog.load(this->filename.get_val()); }

void LangBF::setup_preprocessor()
{
    this->prep.set_check(BF::check::standard);

    if (this->debug.get_val())
    {
        this->prep.set_process(utils::prep::noop);
    }
    else if (this->optimize.get_val())
    {
        this->prep.set_process(BF::prep::optimize);
    }
    else
    {
        this->prep.set_process(BF::prep::remove_unused);
    }
}

void LangBF::setup_interpreter()
{
    BF::InterpreterBase *inter;

    this->inter_fact.set_mem_factory(&this->mem_fact);

    if (this->debug.get_val())
        this->inter_fact.set_inter_class(BF::InterClass::standard_debug);
    else if (this->optimize.get_val())
        this->inter_fact.set_inter_class(BF::InterClass::optimized);
    else
        this->inter_fact.set_inter_class(BF::InterClass::standard);

    switch (this->mem_data_type)
    {
    case BF::MemDataType::bit8:
        inter = this->inter_fact.new_inter<uint8_t>(std::cout, std::cin);
        break;

    case BF::MemDataType::bit16:
        inter = this->inter_fact.new_inter<uint16_t>(std::cout, std::cin);
        break;
    }

    this->inter.reset(inter);
}

bool LangBF::load_options(int argc, const char **argv)
{
#ifdef DEBUG
    std::cout << "[LangBF]: Arguments:\n";
    for (int i = 0; i < argc; i++)
    {
        std::cout << "  [" << i << "]: " << argv[i] << "\n";
    }
    std::cout << std::flush;
#endif // DEBUG

    std::vector<std::string> unrecognised;

    try
    {
        unrecognised = argp::parse(argc, argv, this->opts, 1);
    }
    catch (const std::exception &e)
    {
        throw options_error(e.what());
    }

    if (this->help.get_val())
    {
        argp::print_help(std::cout, argv[0], this->opts);
        return false;
    }

    if (unrecognised.size() != 0)
    {
        std::string msg = "Unrecognised parameters:";
        for (std::string param : unrecognised)
        {
            msg += " \"";
            msg += param;
            msg += "\"";
        }
        throw options_error(msg);
    }

    this->process_options();

    DEBUG_PRINT("[LangBF]: All options parsed.");

    return true;
}

void LangBF::prepare()
try
{
    this->setup_program();
    this->setup_preprocessor();
    this->setup_interpreter();
    this->prep.check(this->prog);
    this->prep.process(this->prog);
}
catch (const std::exception &e)
{
    throw prepare_error(e.what());
}

void LangBF::run()
try
{
    DEBUG_PRINT("[LangBF]: Running the program.");

    this->inter->run(this->prog);
}
catch (const language_error &e)
{
    throw;
}
catch (const std::exception &e)
{
    throw run_error(e.what());
}

LangBF::LangBF()
    : filename("filename", "Load program from this file. (REQUIRED)", true),
      mem_type(
          {"--mem-type"},
          "Set memory type. Available values:\n"
          "  static-safe - static size, bounds checking (default)\n"
          "  static-unsafe - static size, no bounds checking\n"
          "  static-loop - static size, decrementing pointer to the first "
          "element goes to the last one and vice versa\n"
          "  dynamic - incrementing pointer to the last element increases "
          "the memory size (decrementing pointer to the first element results "
          "in runtime error)",
          BF::MemoryType::static_safe),
      mem_size(
          {"--mem-size"},
          "Set memory size for the interpreted program.\n"
          "Default value: 30 000.\n"
          "This option is ignored when using dynamic memory. "
          "(However, it will still cause an error if set to invalid value.)",
          30000),
      mem_cell_size({"--cell-size"},
                    "Set number of bits for every stack entry.\n"
                    "Allowed values: 8 (default), 16",
                    BF::MemDataType::bit8),
      debug({"--debug"}, "Run with debugging. (Breakpoint symbol \"|\")"),
      optimize(
          {"--optimize"},
          "When this option is selected, preprocessor will try to optimize "
          "the program.\n"
          "Warning: This option cannot be used when debugging is "
          "enabled."),
      help({"-h", "--help"}, "Show this help."),
      opts({&filename, &mem_type, &mem_size, &mem_cell_size, &debug, &optimize,
            &help})
{
    this->inter = nullptr;
}
