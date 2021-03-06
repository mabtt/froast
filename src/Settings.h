// Copyright (C) 2011 Oliver Schulz <oliver.schulz@tu-dortmund.de>

// This is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#ifndef FROAST_SETTINGS_H
#define FROAST_SETTINGS_H

#include <vector>
#include <stdint.h>

#include <TString.h>
#include <TEnv.h>
#include <THashList.h>
#include <TDirectory.h>


namespace froast {


class Param {
protected:
	TString m_name;

public:
	operator const TString& () const { return m_name; }

	operator const char* () const { return m_name.Data(); }

	operator std::string () const { return std::string(m_name.Data()); }
	std::string str() const { return std::string(m_name.Data()); }

	Param operator()(int32_t idx) const;
	
	Param();
	Param(const TString &pname);
	virtual ~Param();
};


class Settings {
protected:
	static Settings m_global;

	TEnv *m_env, outgoing;
	bool m_envOwned;
	
public:
	static Settings &global() { return m_global; }

	void getInstances(const TString &pattern, std::vector<int32_t> &instances);

	bool operator()(const char* name, bool dflt, bool saveDflt = true);
	int32_t operator()(const char* name, int32_t dflt, bool saveDflt = true);
	double operator()(const char* name, double dflt, bool saveDflt = true);
	const char* operator()(const char* name, const char* dflt, bool saveDflt = true);
	
	const TEnv* tenv() const { return m_env; }
	TEnv* tenv() { return m_env; }
	const THashList* table() const { return tenv()->GetTable(); }
	THashList* table() { return tenv()->GetTable(); }

	THashList* exportNested(EEnvLevel minLevel = kEnvLocal) const;
	void importNested(const THashList *nested, EEnvLevel level = kEnvLocal, const TString &prefix = "");

	void write(const TString &fileName, EEnvLevel minLevel = kEnvLocal);
	void read(const TString &fileName, EEnvLevel level = kEnvLocal);
	
	std::ostream& write(std::ostream &out, EEnvLevel minLevel = kEnvLocal);

	void read(TDirectory *tdir, const TString &name = "settings");
	void writeToGDirectory(const TString &name = "settings", EEnvLevel minLevel = kEnvLocal);

	std::string toString();
	
	void clear();

	Settings(TEnv* env, bool own = false);

	Settings();
	virtual ~Settings();
};



class GSettings {
public:
	static void getInstances(const TString &pattern, std::vector<int32_t> &instances) { Settings::global().getInstances(pattern, instances); }

	static bool get(const char* name, bool dflt, bool saveDflt = true) { return Settings::global()(name, dflt, saveDflt); }
	static int32_t get(const char* name, int32_t dflt, bool saveDflt = true) { return Settings::global()(name, dflt, saveDflt); }
	static double get(const char* name, double dflt, bool saveDflt = true) { return Settings::global()(name, dflt, saveDflt); }
	static const char* get(const char* name, const char* dflt, bool saveDflt = true) { return Settings::global()(name, dflt, saveDflt); }
};


} // namespace froast


#ifdef __CINT__
#pragma link C++ class froast::Param-;
#pragma link C++ class froast::Settings-;
#pragma link C++ class froast::GSettings-;
#endif

#endif // FROAST_SETTINGS_H
