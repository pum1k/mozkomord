#ifndef CLI_LANGUAGE_HPP_
#define CLI_LANGUAGE_HPP_

class Language
{
 private:
 public:
    virtual bool load_options(int argc, const char **argv) = 0;
    virtual void prepare()                                 = 0;
    virtual void run()                                     = 0;
};

class language_error : public std::exception
{
 protected:
    std::string msg;

 public:
    language_error(const std::string &msg = "language_error") : msg(msg){};

    virtual const char *what() const noexcept override
    {
        return this->msg.c_str();
    };
};

class options_error : public language_error
{
 public:
    options_error(const std::string &msg)
        : language_error(std::string("Error while parsing options:\n  ") + msg)
    {
    }
};

class prepare_error : public language_error
{
 public:
    prepare_error(const std::string &msg)
        : language_error(std::string("Error while preparing environment:\n  ") +
                         msg)
    {
    }
};

class run_error : public language_error
{
 public:
    run_error(const std::string &msg)
        : language_error(std::string("Error while running the program:\n  ") +
                         msg)
    {
    }
};

#endif // CLI_LANGUAGE_HPP_
