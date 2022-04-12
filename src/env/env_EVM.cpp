#include "maat/env/env_EVM.hpp"
#include "maat/engine.hpp"

namespace maat{
namespace env{
namespace EVM{

int Stack::size() const
{
    return _stack.size();
}

int Stack::_pos_to_idx(int pos) const
{
    int idx = size()-1-pos;
    if (idx < 0 or idx >= size())
        throw env_exception("EVM::Stack: requested invalid element posiion");
    return idx;
}

const Value& Stack::get(int pos) const
{
    int idx = _pos_to_idx(pos);
    return _stack[idx];
}

void Stack::pop()
{
    if (size() == 0)
        throw env_exception("EVM::Stack::pop(): stack is empty");
    _stack.pop_back();
}

void Stack::set(const Value& value, int pos)
{
    int idx = _pos_to_idx(pos);
    _stack[idx] = value;
}
void Stack::push(const Value& value)
{
    _stack.push_back(value);
}

std::ostream& operator<<(std::ostream& os, const Stack& stack)
{
    os << "EVM Stack:\n";
    for (const auto& val : stack._stack)
        os << val << "\n";
    return os;
}

Contract::Contract(Value addr): address(addr) {}


EthereumEmulator::EthereumEmulator(): EnvEmulator(Arch::Type::EVM, OS::NONE)
{
    _init();
}

void EthereumEmulator::_init()
{
    EnvEmulator::_init(Arch::Type::NONE, OS::NONE);
}

int EthereumEmulator::add_contract(contract_t contract)
{
    int uid = _uid_cnt++;
    const auto& exists = _contracts.find(uid);
    if (exists != _contracts.end())
        throw env_exception("Ethereum: add_contract(): uid already used !");
    _contracts[uid] = contract;
    return uid;
}

contract_t EthereumEmulator::get_contract_by_uid(int uid) const
{
    auto res = _contracts.find(uid);
    if (res == _contracts.end())
        throw env_exception("Ethereum: get_contract_by_uid(): no corresponding contract");
    return res->second;
}


serial::uid_t EthereumEmulator::class_uid() const
{
    return serial::ClassId::ENV_ETHEREUM_EMULATOR;
}

void EthereumEmulator::dump(serial::Serializer& s) const
{
    // TODO 
}

void EthereumEmulator::load(serial::Deserializer& d)
{
   // TODO
}


std::shared_ptr<EthereumEmulator> get_ethereum(MaatEngine& engine)
{
    std::shared_ptr<EthereumEmulator> res = std::dynamic_pointer_cast<EthereumEmulator>(
        engine.env
    );
    return res;
}

contract_t get_contract_for_engine(MaatEngine& engine)
{
    return get_ethereum(engine)->get_contract_by_uid(engine.process->pid);
}

} // namespace EVM
} // namespace env
} // namespace maat