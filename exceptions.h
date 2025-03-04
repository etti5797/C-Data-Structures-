
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>

class invalidInput : public std::exception{};
class employeeAlreadyExist : public std::exception{};
class employeeDoesntExist : public std::exception{};
class SalaryDoesntExist : public std::exception{};
class SalaryAlreadyExist : public std::exception{};

#endif /*EXCEPTIONS_H*/

