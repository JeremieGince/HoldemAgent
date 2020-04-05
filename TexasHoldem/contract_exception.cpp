/**
 * \file contract_exception.cpp
 * \brief Implementation of ContractExeption class
 * \author Samuel Ouellet
 */
#include "contract_exception.h"
#include <sstream>
#include <utility>

using namespace std;
/**
 * \brief Constructor of base class ContractException
 * \param p_file  file where the error occured
 * \param p_line int corresponding to the line where the error occured
 * \param p_msg error message
 * \param p_expr expression corresponding to what failed
 */
ContractException::ContractException(std::string p_file, unsigned int p_line,
		std::string p_expr, const std::string& p_msg) :
	logic_error(p_msg), m_expression(std::move(p_expr)), m_file(std::move(p_file)), m_line(p_line)
{
}
/**
 * \brief Construct message
 * \return string corresponding to the message
 */
std::string ContractException::reqTextException() const
 {
 // ---  Prepare message
 ostringstream os;
 os << "Message : " << what() << endl;
 os << "Filie : " << m_file << endl;
 os << "Line   : " << m_line << endl;
 os << "Test    : " << m_expression << endl;

 return os.str();
 }
/**
 * \brief Constructor AssertionException class
 * \param p_file  file where the error occured
 * \param p_line int corresponding to the line where the error occured
 * \param p_msg error message
 * \param p_expr expression corresponding to what failed
 */
 AssertionException::AssertionException(std::string p_file, unsigned int p_line,
 std::string p_expr)
 : ContractException(p_file, p_line, p_expr, "Assertion Error")
 {
 }

 /**
 * \brief  Constructor of PreconditionException class
 * \param p_file  file where the error occured
 * \param p_line int corresponding to the line where the error occured
 * \param p_expr expression corresponding to what failed
 */
 PreconditionException::PreconditionException(std::string p_file, unsigned int p_line,
 std::string p_expr)
 : ContractException(p_file, p_line, p_expr, "Precondition Error")
 {
 }
/**
* \brief  Constructor of PostconditionException class
* \param p_file  file where the error occured
* \param p_line int corresponding to the line where the error occured
* \param p_expr expression corresponding to what failed
*/
 PostconditionException::PostconditionException(std::string p_file, unsigned int p_line,
 std::string p_expr)
 : ContractException(std::move(p_file), p_line, std::move(p_expr), "Postcondition Error")
 {
 }

/**
* \brief  Constructor of InvariantException class
* \param p_file  file where the error occured
* \param p_line int corresponding to the line where the error occured
* \param p_expr expression corresponding to what failed
*/
 InvariantException::InvariantException(std::string p_file, unsigned int p_line,
 std::string p_expr)
 : ContractException(std::move(p_file), p_line, std::move(p_expr), "Invariant Error")
 {
 }
