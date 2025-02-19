

#include "lua.hpp"

#include <cstring>
#include <iostream>

// #include "include/third_party/catch.hpp"
#include "include/operators/unary_arithmetic.hpp"
#include "include/operators/relational.hpp"
#include "include/operators/io_stream.hpp"
#include "include/operators/increment_decrement.hpp"
#include "include/operators/binary_arithmetic.hpp"
#include "include/operators/assignment.hpp"
#include "include/operators/arithmetic_assignment.hpp"
#include "include/functions/utility.hpp"
#include "include/functions/math.hpp"
#include "include/functions/random.hpp"
#include "include/functions/conversion.hpp"
#include "include/constructors/constructors.hpp"
#include "include/BigInt.hpp"

static BigInt *newL_BigInt(lua_State *L)
{
	BigInt *b = (BigInt* )lua_newuserdata(L, sizeof(BigInt));
	memset(b, 0, sizeof(BigInt));
	b->Init();
	luaL_setmetatable(L, BIG_INT);
	return b;
}

static BigInt *newL_BigIntd(lua_State *L, double d)
{
	BigInt *b = (BigInt* )lua_newuserdata(L, sizeof(BigInt));
	memset(b, 0, sizeof(BigInt));
	try
	{
		std::string ds = std::to_string(d);
		b->Init(ds);
	}
	catch (const std::exception &e)
	{
		luaL_error(L, "to bigint error %s", e.what());
	}
	luaL_setmetatable(L, BIG_INT);
	return b;
}

static BigInt *newL_BigInt(lua_State *L, long long ll)
{
	BigInt *b = (BigInt* )lua_newuserdata(L, sizeof(BigInt));
	memset(b, 0, sizeof(BigInt));
	b->Init(ll);
	luaL_setmetatable(L, BIG_INT);
	return b;
}

static BigInt *newL_BigInt(lua_State *L, std::string s)
{
	BigInt *b = (BigInt* )lua_newuserdata(L, sizeof(BigInt));
	memset(b, 0, sizeof(BigInt));
	try
	{
		b->Init(s);
	}
	catch (const std::exception &e)
	{
		luaL_error(L, "to bigint error %s", e.what());
	}
	luaL_setmetatable(L, BIG_INT);
	return b;
}

static BigInt *newL_BigInt(lua_State *L, BigInt *big)
{
	BigInt *b = (BigInt* )lua_newuserdata(L, sizeof(BigInt));
	memset(b, 0, sizeof(BigInt));
	b->Init(*big);
	luaL_setmetatable(L, BIG_INT);
	return b;
}

// construct
static int L_tobigint(lua_State *L)
{
	int t = lua_type(L, 1);
	switch (t)
	{
	case LUA_TNUMBER:
		if (lua_isinteger(L, 1))
			newL_BigInt(L, lua_tointeger(L, 1));
		else
			newL_BigIntd(L, lua_tonumber(L, 1));
		break;
	case LUA_TSTRING:
		newL_BigInt(L, lua_tostring(L, 1));
		break;
	case LUA_TUSERDATA:
		BigInt *p;
		p = (BigInt* )luaL_testudata(L, 1, BIG_INT);
		if (p != NULL)
			newL_BigInt(L, p);
		else
			return 0;
		break;
	default:
		newL_BigInt(L);
		break;
	}
	return 1;
}

BigInt *L_trans_bigint(lua_State *L, size_t idx)
{
	int t = lua_type(L, 1);
	BigInt *b;
	switch (t)
	{
	case LUA_TNUMBER:
		if (lua_isinteger(L, 1))
			b = new BigInt(lua_tointeger(L, 1));
		else
		{
			try
			{
				std::string ds = std::to_string(lua_tonumber(L, 1));
				b->Init(ds);
			}
			catch (const std::exception &e)
			{
				luaL_error(L, "to bigint error %s", e.what());
			}
		}
		break;
	case LUA_TSTRING:
		try
		{
			b->Init(lua_tostring(L, 1));
		}
		catch (const std::exception &e)
		{
			luaL_error(L, "to bigint error %s", e.what());
		}
		break;
	case LUA_TUSERDATA:
		b = (BigInt* )luaL_testudata(L, 1, BIG_INT);
		if (b == NULL)
			luaL_typeerror(L, t, BIG_INT);
		break;
	default:
		luaL_typeerror(L, t, BIG_INT);\
		break;
	}
	return b;
}

static int L_tostring(lua_State *L)
{
	int t = lua_type(L, 1);
	BigInt *p;
	if (t == LUA_TUSERDATA)
	{
		p = (BigInt* )luaL_testudata(L, 1, BIG_INT);
		if (p != NULL)
		{
			std::string s = p->to_string();
			lua_pushstring(L, s.c_str());
			return 1;
		}
	}
	else
	{
		luaL_typeerror(L, t, BIG_INT);
	}
	lua_pushstring(L, "error value");
	return 1;
}

static int L_add(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1), *r = L_trans_bigint(L, 2);
	BigInt result;
	result = *l + *r;
	newL_BigInt(L, &result);
	return 1;
}

static int L_sub(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1), *r = L_trans_bigint(L, 2);
	BigInt result;
	result = *l - *r;
	newL_BigInt(L, &result);
	return 1;
}

static int L_mul(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1), *r = L_trans_bigint(L, 2);
	BigInt result;
	result = (*l) * (*r);
	newL_BigInt(L, &result);
	return 1;
}

static int L_div(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1), *r = L_trans_bigint(L, 2);
	BigInt result;
	result = (*l) / (*r);
	newL_BigInt(L, &result);
	return 1;
}

static int L_mod(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1), *r = L_trans_bigint(L, 2);
	BigInt result;
	result = (*l) % (*r);
	newL_BigInt(L, &result);
	return 1;
}

static int L_unm(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1);
	BigInt result;
	result = - (*l);
	newL_BigInt(L, &result);
	return 1;
}

static int L_eq(lua_State *L)
{
	BigInt* l = L_trans_bigint(L, 1); 
	BigInt* r = L_trans_bigint(L, 2);
	//bool result;
	//result = (l == r);
	lua_pushboolean(L, l->sign == r->sign);
	lua_pushboolean(L, l->value == r->value);
	return 2;
}

static int L_lt(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1), *r = L_trans_bigint(L, 2);
	bool result;
	result = (*l) < (*r);
	lua_pushboolean(L, result);
	return 1;
}

static int L_le(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1), *r = L_trans_bigint(L, 2);
	bool result;
	result = (*l) <= (*r);
	lua_pushboolean(L, result);
	return 1;
}

static int L_from_u64(lua_State *L)
{
	BigInt* b = newL_BigInt(L);
	b->from_u64((uint64_t)lua_tointeger(L, 1));
	return 1;
}

static int L_to_u64(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1);
	lua_pushnumber(L, l->to_u64());
	return 1;
}

static int L_abs(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1);
	BigInt* b = newL_BigInt(L, l);
	b->sign = '+';
	return 1;
}

static int L_compare(lua_State *L)
{
	BigInt *l = L_trans_bigint(L, 1), *r = L_trans_bigint(L, 2);
	if ( *l == *r )
		lua_pushnumber(L, 0);
	else if ( *l > *r )
		lua_pushnumber(L, 1);
	else
		lua_pushnumber(L, -1);	
	
	return 1;
}
	// __add	对应的运算符 '+'.
	// __sub	对应的运算符 '-'.
	// __mul	对应的运算符 '*'.
	// __div	对应的运算符 '/'.
	// __mod	对应的运算符 '%'.
	// __unm	对应的运算符 '-'.
	// __concat	对应的运算符 '..'.
	// __eq	对应的运算符 '=='.
	// __lt	对应的运算符 '<'.
	// __le	对应的运算符 '<='.
	// __tostring

	static const luaL_Reg methods[] = {
		//{ "decode", json_decode },
		{"tobigint", L_tobigint},
#define APPEND(name) { "__"#name, L_##name}, {#name, L_##name},
		APPEND(tostring)
		APPEND(add)
		APPEND(sub)
		APPEND(mul)
		APPEND(div)
		APPEND(mod)
		APPEND(unm)
		APPEND(eq)
		APPEND(lt)
		APPEND(le)
#undef APPEND
		{"__idiv", L_div},
		{"abs", L_abs},
		{"compare", L_compare},
		{"tou64", L_to_u64},
		{NULL, NULL}
	};

extern "C"
{

	LUALIB_API int luaopen_bigint(lua_State *L)
	{
		if (luaL_newmetatable(L, BIG_INT))
		{
			luaL_setfuncs(L, methods, 0);
			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");
			lua_createtable(L, 0, 1);
			lua_pushcclosure(L, (L_tobigint), 0);
			lua_setfield(L, -2, "__call");
			lua_setmetatable(L, -2);
		}
		return 1;
	}
}
