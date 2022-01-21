#include "language.hpp"

void LangBF::print_help()
{
    std::cout << "Usage: bfi.exe [options]\n"
              << "Options:\n";
    this->parser.print_help(std::cout);
}

void LangBF::process_options()
{
    // File
    if (this->default_s == this->parser.get_string("file"))
    {
        throw options_error(
            "You must specify file to run. (options \"-f\" or \"--file\")");
    }

    // Memory type
    const std::string &mem_type_s = this->parser.get_string("mem-type");
    if (mem_type_s == "static-safe" || mem_type_s == "")
        this->mem_fact.set_mem_type(BF::MemoryType::STATIC_SAFE);
    else if (mem_type_s == "static-unsafe")
        this->mem_fact.set_mem_type(BF::MemoryType::STATIC_UNSAFE);
    else if (mem_type_s == "static-loop")
        this->mem_fact.set_mem_type(BF::MemoryType::STATIC_LOOP);
    else if (mem_type_s == "dynamic")
        this->mem_fact.set_mem_type(BF::MemoryType::DYNAMIC);
    else
    {
        throw options_error("Invalid value for \"--mem-type\" option.");
    }

    // Memory size
    const int &stack_size = this->parser.get_int("mem-size");
    if (!(stack_size == this->default_i || stack_size > 0))
    {
        throw options_error("Invalid value for \"--mem-size\" option.");
    }
}

void LangBF::setup_program()
{
    this->prog.load(this->parser.get_string("file"));
}

void LangBF::setup_preprocessor()
{
    this->prep.set_check(BF::check::standard);
    this->prep.set_process(BF::prep::remove_unused);
}

void LangBF::setup_interpreter()
{
    int mem_size_i       = this->parser.get_int("mem-size");
    std::size_t mem_size = (mem_size_i < 0) ? 30000 : mem_size_i;
    this->inter.reset(new BF::Interpreter<char>(
        std::cout, std::cin, this->mem_fact.new_mem<char>(mem_size)));
}

LangBF::LangBF()
    : parser({
          // File
          {"file",
           {"-f", "--file"},
           argp::OptionType::STRING,
           "Load program from this file. (REQUIRED)"},
          // Memory type
          {"mem-type",
           {"--mem-type"},
           argp::OptionType::INT,
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
           "(However, it will cause an error if set to invalid value.)"},
          // Help
          {"help", {"-h", "--help"}, argp::OptionType::FLAG, "Show this help."},
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
        unrecognised = !this->parser.parse(argc, argv, 0);
    }
    catch (const std::exception &e)
    {
        throw options_error(e.what());
    }

    if (this->parser.get_flag("help"))
    {
        this->print_help();
        return false;
    }
    if (unrecognised)
    {
        // TODO: Create better error message
        throw options_error("Unrecognised parameters!");
    }

    this->process_options();

    DEBUG_PRINT("[LangBF]: All options parsed.");

    return true;
}

void LangBF::prepare()
{
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
}

void LangBF::run()
{
#ifdef DEBUG
    std::cout << "[LangBF]: This program will be run:\n";
    for (auto c : this->prog.get_const())
    {
        std::cout << c;
    }
    std::cout << "\n#### End of program." << std::endl;
#endif // DEBUG
    this->inter->run(this->prog);
}
