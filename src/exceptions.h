/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
// CONTRIBUTORS AND COPYRIGHT HOLDERS (c) 2013:
// Dag Robøle (BM-2DAS9BAs92wLKajVy9DS1LFcDiey5dxp5c)

#ifndef BM_EXCEPTIONS_H
#define BM_EXCEPTIONS_H

#include <exception>

namespace bm {

class Exception : public std::exception
{
public:

    explicit Exception(const char* file, int line, const char* message) throw()
        : m_file(file), m_message(message), m_line(line) {}

    virtual ~Exception() throw() {}

    virtual const char* file() const throw() { return m_file; }
    virtual int line() const throw() { return m_line; }
    virtual const char* what() const throw() { return m_message; }

private:

    const char *m_file, *m_message;
    int m_line;
};

class RangeException : public Exception
{
public:

    explicit RangeException(const char* file, int line, const char* message) throw()
        : Exception(file, line, message) {}
    virtual ~RangeException() throw() {}
};

class SizeException : public Exception
{
public:

    explicit SizeException(const char* file, int line, const char* message) throw()
        : Exception(file, line, message) {}
    virtual ~SizeException() throw() {}
};

} // namespace bm

#endif
