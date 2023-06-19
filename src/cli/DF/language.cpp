#include "language.hpp"

void LangDF::setup_preprocessor()
{
    this->prep.set_check(DF::check::standard);

    this->prep.set_process(
        utils::prep::get_unused_remover("+-<>[].,/\\^v{}:;"));
}

void LangDF::setup_interpreter()
{
    BF::InterpreterBase *interpreter;

    switch (this->mem_data_type)
    {
    case BF::MemDataType::bit8:
        interpreter = new DF::DFInterpreter<uint8_t>(
            std::cout, std::cin, this->mem_fact.new_mem<uint8_t>(),
            this->mem_fact.new_mem<uint8_t>());
        break;

    case BF::MemDataType::bit16:
        interpreter = new DF::DFInterpreter<uint16_t>(
            std::cout, std::cin, this->mem_fact.new_mem<uint16_t>(),
            this->mem_fact.new_mem<uint16_t>());
        break;

    default:
        throw language_error("unsupported memory cell size");
        break;
    }
    this->inter.reset(interpreter);
}

LangDF::LangDF()
{
    this->opts = argp::OptionsList(
        {&filename, &mem_type, &mem_size, &mem_cell_size, &help});
}
