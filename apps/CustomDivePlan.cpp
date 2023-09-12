////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CustomDivePlan.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "core/deco.h"
#include "core/dive.h"
#include "core/divelog.h"
#include "core/file.h"
#include "core/event.h"
#include "core/divecomputer.h"
#include "core/planner.h"
#include "core/qthelper.h"
#include "core/subsurfacestartup.h"
#include "core/units.h"
#include "core/divelist.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "commands/command.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <QDebug>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DEBUG 1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// testing the dive plan algorithm
// static struct dive dive = { 0 };
// static struct decostop stoptable[60];
// static struct deco_state test_deco_state;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern bool plan(struct deco_state *ds, struct diveplan *diveplan, struct dive *dive, int timestep, struct decostop *decostoptable, struct deco_state **cached_datap, bool is_planner, bool show_disclaimer);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CustomDivePlan::loadLogFiles (const std::string & fname)
{
	if (true) // Method 1
		parse_file (fname.c_str (), &divelog);
	else // Method 2 -- has a linker error! But the false-if ignores it
		Command::importDives (&divelog, IMPORT_MERGE_ALL_TRIPS, fname.c_str ());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct diveplan * CustomDivePlan::createDivePlanFromComputer (const struct divecomputer & dc)
{
	struct diveplan * divePlan = new diveplan ();
	std::vector <FastDiveInfo> fastDiveInfoVec;
	fastDiveInfoVec.reserve (dc.samples);
	for (int i = 0; i < dc.samples - 1; i++)
	{
		const struct sample & samp = dc.sample [i];
		fastDiveInfoVec.push_back (FastDiveInfo::fromSample (samp));
		if ((i == 0)) continue;
		const FastDiveInfo & curr = fastDiveInfoVec [i];
		const FastDiveInfo & last = fastDiveInfoVec [i-1];
		plan_add_segment (divePlan, /*duration*/ curr.time.seconds - last.time.seconds, /*depth*/ last.depth.mm, /*cylinder*/ 0, /*po2*/ 0, /*entered*/ 1, /*dive mode*/ OC);
	}

	return divePlan;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CustomDivePlan::planDeco (diveplan * divePlan, struct dive * dive)
{
	struct decostop stopTable [60];
	struct deco_state decoState;
	struct deco_state * cache = NULL;
	plan (&decoState, divePlan, dive, 60, stopTable, &cache, 1, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main ()
{
	CustomDivePlan cdp;
	const std::string fname = SUBSURFACE_SOURCE "/dives/dives_seac_oddity.ssrf";
	cdp.loadLogFiles (fname);
	for (int i = 0; i < 1; ++i)
	{
		struct dive * dive = get_dive (i);
		assert (dive != nullptr);
		const struct divecomputer & dc = dive->dc;
		struct diveplan * divePlan = cdp.createDivePlanFromComputer (dc);
		cdp.planDeco (divePlan, dive);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
