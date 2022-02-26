#include "language.hpp"

void LangBF::print_help(const char *prog_name)
{
    std::cout << "Usage: " << prog_name << " [options]\n"
              << "Options:\n";
    this->parser.print_help(std::cout);
}

void LangBF::process_options()
{
    // File
    // This option is queried later in the program
    if (this->default_s == this->parser.get_string("file"))
    {
        throw options_error(
            "You must specify file to run. (options \"-f\" or \"--file\")");
    }

    // Memory type
    const std::string &mem_type_s = this->parser.get_string("mem-type");
    if (mem_type_s == "static-safe" || mem_type_s == "")
        this->mem_fact.set_mem_type(BF::MemoryType::static_safe);
    else if (mem_type_s == "static-unsafe")
        this->mem_fact.set_mem_type(BF::MemoryType::static_unsafe);
    else if (mem_type_s == "static-loop")
        this->mem_fact.set_mem_type(BF::MemoryType::static_loop);
    else if (mem_type_s == "dynamic")
        this->mem_fact.set_mem_type(BF::MemoryType::dynamic);
    else
    {
        throw options_error("Invalid value for \"--mem-type\" option.");
    }

    // Memory size
    const int &stack_size = this->parser.get_int("mem-size");
    if (stack_size == this->default_i)
        this->mem_fact.set_mem_size(30000);
    else if (stack_size > 0)
        this->mem_fact.set_mem_size(stack_size);
    else
    {
        throw options_error("Invalid value for \"--mem-size\" option.");
    }

    // Debug mode
    // No check required
    // This option is queried later in the program

    // Mem cell size
    const int &cell_size = this->parser.get_int("cell-size");
    if (cell_size == this->default_i || cell_size == 8)
        this->mem_data_type = BF::MemDataType::bit8;
    else if (cell_size == 16)
        this->mem_data_type = BF::MemDataType::bit16;
    else
    {
        throw options_error("Invalid value for \"--cell-size\" option.");
    }

    // Optimizations
    // This option is queried later in the program
    // if both debugging and optimizations are enabled, throw error
    if (this->parser.get_flag("debug") && this->parser.get_flag("optimize"))
    {
        throw options_error(
            "Debugging and optimizations cannot be enabled both at the same "
            "time.");
    }
}

void LangBF::setup_program()
{
    this->prog.load(this->parser.get_string("file"));
}

void LangBF::setup_preprocessor()
{
    this->prep.set_check(BF::check::standard);

    if (this->parser.get_flag("debug"))
    {
        this->prep.set_process(BF::prep::noop);
    }
    else if (this->parser.get_flag("optimize"))
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

    if (this->parser.get_flag("debug"))
        this->inter_fact.set_inter_class(BF::InterClass::standard_debug);
    else if (this->parser.get_flag("optimize"))
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

LangBF::LangBF()
    : parser({
          // File
          {"file",
           {"-f", "--file"},
           argp::OptionType::STRING,
           "Load program from this file. (REQUIRED)\n"},
          // Memory type
          {"mem-type",
           {"--mem-type"},
           argp::OptionType::STRING,
           "Set memory type. Available values:\n"
           "      static-safe - static size, bounds checking (default)\n"
           "      static-unsafe - static size, no bounds checking\n"
           "      static-loop - static size, decrementing pointer to the first "
           "element goes to the last one and vice versa\n"
           "      dynamic - incrementing pointer to the last element increases "
           "the memory size (decrementing pointer to the first element results "
           "in runtime error)\n"},
          // Memory size
          {"mem-size",
           {"--mem-size"},
           argp::OptionType::INT,
           "Set memory size for the interpreted program.\n"
           "      Default: 30 000.\n"
           "      This option is ignored when using dynamic memory. "
           "(However, it will cause an error if set to invalid value.)\n"},
          // Debug mode
          {"debug",
           {"--debug"},
           argp::OptionType::FLAG,
           "Run with debugging. (Breakpoint symbol \"|\")\n"},
          // Mem cell size
          {"cell-size",
           {"--cell-size"},
           argp::OptionType::INT,
           "Set number of bits for every stack entry.\n"
           "      Allowed values: 8 (default), 16\n"},
          // Optimizations
          {"optimize",
           {"--optimize"},
           argp::OptionType::FLAG,
           "When this option is selected, preprocessor will try to optimize "
           "the program.\n"
           "      Warning: This option cannot be used when debugging is "
           "enabled.\n"},
          // Help
          {"help",
           {"-h", "--help"},
           argp::OptionType::FLAG,
           "Show this help.\n"},
      })
{
    this->default_b = false;
    this->default_s = "";
    this->default_i = -1;

    this->inter = nullptr;
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

    this->parser.set_defaults(this->default_b, this->default_s,
                              this->default_i);

    bool unrecognised;

    try
    {
        unrecognised = !this->parser.parse(argc, argv, 1);
    }
    catch (const std::exception &e)
    {
        throw options_error(e.what());
    }

    if (this->parser.get_flag("help"))
    {
        this->print_help(argv[0]);
        return false;
    }
    if (unrecognised)
    {
        std::string msg = "Unrecognised parameters:";
        for (std::string param : this->parser.get_unrecognised())
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
#ifdef DEBUG
    DEBUG_PRINT("[LangBF]: Running the program.");
#endif // DEBUG
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
