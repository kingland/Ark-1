#ifndef ark_vm_value
#define ark_vm_value

#include <vector>
#include <variant>
#include <string>
#include <cinttypes>
#include <iostream>
#include <memory>
#include <functional>
#include <utility>

#include <Ark/VM/Types.hpp>
#include <Ark/VM/Closure.hpp>
#include <Ark/Exceptions.hpp>
#include <Ark/VM/UserType.hpp>

namespace Ark
{
    class VM;
}

namespace Ark::internal
{
    enum class ValueType
    {
        List,
        Number,
        String,
        PageAddr,
        CProc,
        Closure,
        User,

        Nil,
        True,
        False,
        Undefined
    };

    class Frame;

    class Value
    {
    public:
        using ProcType = std::function<Value (std::vector<Value>&)>;
        using Iterator = std::vector<Value>::const_iterator;
        using Value_t  = std::variant<double, std::string, PageAddr_t, ProcType, Closure, UserType, std::vector<Value>>;

        /**
         * @brief Construct a new Value object
         * 
         */
        Value();

        /**
         * @brief Construct a new Value object
         * 
         * @param type the value type which is going to be held
         */
        Value(ValueType type);

        /**
         * @brief Construct a new Value object as a Number
         * 
         * @param value 
         */
        Value(int value);

        /**
         * @brief Construct a new Value object as a Number
         * 
         * @param value 
         */
        Value(float value);

        /**
         * @brief Construct a new Value object as a Number
         * 
         * @param value 
         */
        Value(double value);

        /**
         * @brief Construct a new Value object as a String
         * 
         * @param value 
         */
        Value(const std::string& value);

        /**
         * @brief Construct a new Value object as a String
         * 
         * @param value 
         */
        Value(std::string&& value);

        /**
         * @brief Construct a new Value object as a Function
         * 
         * @param value 
         */
        Value(PageAddr_t value);

        /**
         * @brief Construct a new Value object from a C++ function
         * 
         * @param value 
         */
        Value(Value::ProcType value);

        /**
         * @brief Construct a new Value object as a List
         * 
         * @param value 
         */
        Value(std::vector<Value>&& value);

        /**
         * @brief Construct a new Value object as a Closure
         * 
         * @param value 
         */
        Value(Closure&& value);

        /**
         * @brief Construct a new Value object as a UserType
         * 
         * @param value 
         */
        Value(UserType&& value);

        /**
         * @brief Return the value type
         * 
         * @return ValueType 
         */
        inline ValueType valueType() const;

        /**
         * @brief Check if a function is held
         * 
         * @return true on success
         * @return false on failure
         */
        inline bool isFunction() const;

        /**
         * @brief Return the stored number
         * 
         * @return double 
         */
        inline double number() const;

        /**
         * @brief Return the stored string
         * 
         * @return const std::string& 
         */
        inline const std::string& string() const;

        /**
         * @brief Return the stored list
         * 
         * @return const std::vector<Value>& 
         */
        inline const std::vector<Value>& const_list() const;

        /**
         * @brief Return the stored user type
         * 
         * @return const UserType& 
         */
        inline const UserType& usertype() const;

        /**
         * @brief Return the stored list as a reference
         * 
         * @return std::vector<Value>& 
         */
        std::vector<Value>& list();

        /**
         * @brief Return the stored string as a reference
         * 
         * @return std::string& 
         */
        std::string& string_ref();

        /**
         * @brief Return the stored user type as a reference
         * 
         * @return UserType& 
         */
        UserType& usertype_ref();

        /**
         * @brief Add an element to the list held by the value (if the value type is set to list)
         * 
         * @param value 
         */
        void push_back(const Value& value);

        /**
         * @brief Add an element to the list held by the value (if the value type is set to list)
         * 
         * @param value 
         */
        void push_back(Value&& value);

        /**
         * @brief Resolve a function call (works only if the object is a function) with given arguments
         * 
         * Needed by C function bindings in modules, to resolve the value of a function call
         * 
         * @tparam Args 
         * @param args 
         * @return Value 
         */
        template <typename... Args>
        Value resolve(Args&&... args) const;

        friend std::ostream& operator<<(std::ostream& os, const Value& V);
        friend inline bool operator==(const Value& A, const Value& B);
        friend inline bool operator<(const Value& A, const Value& B);
        friend inline bool operator!(const Value& A);

        friend class Ark::VM;

    private:
        Value_t m_value;
        ValueType m_type;
        bool m_const;
        Ark::VM* m_vm = nullptr;

        // private getters only for the virtual machine

        /**
         * @brief Return the page address held by the value
         * 
         * @return PageAddr_t 
         */
        inline PageAddr_t pageAddr() const;

        /**
         * @brief Return the C Function held by the value
         * 
         * @return const ProcType& 
         */
        inline const ProcType& proc() const;

        /**
         * @brief Return the closure held by the value
         * 
         * @return const Closure& 
         */
        inline const Closure& closure() const;

        /**
         * @brief Return a reference to the closure held by the value
         * 
         * @return Closure& 
         */
        Closure& closure_ref();

        /**
         * @brief Register a pointer to the virtual machine, needed to resolve function calls
         * 
         * @param vm 
         */
        void registerVM(Ark::VM* vm);
    };

    #include "inline/Value.inl"
}

#endif