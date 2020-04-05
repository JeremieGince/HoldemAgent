/**
 * \file   contract_exception.h
 * \brief  Files contains exception calss with its daughter classes
 * \author Samuel Ouellet (111 233 977
 */

#ifndef CONTRACT_EXCEPTION_H
#define CONTRACT_EXCEPTION_H

#include <string>
#include <stdexcept>

/**
 * \class ContratException
 * \brief Classe de base des exceptions de contrat.
 */
class ContractException: public std::logic_error
{
public:
    ContractException(std::string p_file, unsigned int p_line,
			std::string p_expr, const std::string& p_msg);
	~ContractException() throw ()
	{
	}
	;
	std::string reqTextException() const;

private:
	std::string m_expression;
	std::string m_file;
	unsigned int m_line;
};
/**
 * \class AssertionException
 * \brief Class for assertion errors
 */

class AssertionException: public ContractException
{
public:
	AssertionException(std::string, unsigned int, std::string);
};
/**
 * \class PreconditionException
 * \brief class for precondition errors.
 */

class PreconditionException: public ContractException
{
public:
	PreconditionException(std::string, unsigned int, std::string);
};
/**
 * \class PostconditionException
 * \brief class for postcondition errors.
 */
class PostconditionException: public ContractException
{
public:
	PostconditionException(std::string, unsigned int, std::string);
};

/**
 * \class InvariantException
 * \brief Class for invariant errors .
 */
class InvariantException: public ContractException
{
public:
	InvariantException(std::string, unsigned int, std::string);
};


#  define INVARIANTS() \
      verifieInvariant();

#  define ASSERTION(f)     \
      if (!(f)) throw AssertionException(__FILE__,__LINE__, #f);
#  define PRECONDITION(f)  \
      if (!(f)) throw PreconditionException(__FILE__, __LINE__, #f);
#  define POSTCONDITION(f) \
      if (!(f)) throw PostconditionException(__FILE__, __LINE__, #f);
#  define INVARIANT(f)   \
      if (!(f)) throw InvariantException(__FILE__,__LINE__, #f);


#endif  // --- ifndef CONTRATEXCEPTION_H_DEJA_INCLU

