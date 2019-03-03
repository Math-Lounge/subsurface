// SPDX-License-Identifier: GPL-2.0
#ifndef DIVESITE_H
#define DIVESITE_H

#include "units.h"
#include "taxonomy.h"
#include <stdlib.h>

#ifdef __cplusplus
#include <QString>
#include <QObject>
extern "C" {
#else
#include <stdbool.h>
#endif

struct dive_site
{
	uint32_t uuid;
	char *name;
	location_t location;
	char *description;
	char *notes;
	struct taxonomy_data taxonomy;
};

typedef struct dive_site_table {
	int nr, allocated;
	struct dive_site **dive_sites;
} dive_site_table_t;

extern struct dive_site_table dive_site_table;

static inline struct dive_site *get_dive_site(int nr, struct dive_site_table *ds_table)
{
	if (nr >= ds_table->nr || nr < 0)
		return NULL;
	return ds_table->dive_sites[nr];
}

/* iterate over each dive site */
#define for_each_dive_site(_i, _x, _ds_table) \
	for ((_i) = 0; ((_x) = get_dive_site(_i, _ds_table)) != NULL; (_i)++)

int get_divesite_idx(const struct dive_site *ds, struct dive_site_table *ds_table);
struct dive_site *get_dive_site_by_uuid(uint32_t uuid, struct dive_site_table *ds_table);
void dive_site_table_sort(struct dive_site_table *ds_table);
void add_dive_site_to_table(struct dive_site *ds, struct dive_site_table *ds_table);
void remove_dive_site_from_table(struct dive_site *ds, struct dive_site_table *ds_table);
void register_dive_site(struct dive_site *ds);
void unregister_dive_site(struct dive_site *ds);
struct dive_site *alloc_or_get_dive_site(uint32_t uuid, struct dive_site_table *ds_table);
struct dive_site *alloc_dive_site();
int nr_of_dives_at_dive_site(struct dive_site *ds, bool select_only);
bool is_dive_site_used(struct dive_site *ds, bool select_only);
void free_dive_site(struct dive_site *ds);
void delete_dive_site(struct dive_site *ds, struct dive_site_table *ds_table);
struct dive_site *create_dive_site(const char *name, timestamp_t divetime, struct dive_site_table *ds_table);
struct dive_site *create_dive_site_with_gps(const char *name, const location_t *, timestamp_t divetime, struct dive_site_table *ds_table);
struct dive_site *get_dive_site_by_name(const char *name, struct dive_site_table *ds_table);
struct dive_site *get_dive_site_by_gps(const location_t *, struct dive_site_table *ds_table);
struct dive_site *get_dive_site_by_gps_and_name(char *name, const location_t *, struct dive_site_table *ds_table);
struct dive_site *get_dive_site_by_gps_proximity(const location_t *, int distance, struct dive_site_table *ds_table);
struct dive_site *get_same_dive_site(const struct dive_site *);
bool dive_site_is_empty(struct dive_site *ds);
void copy_dive_site_taxonomy(struct dive_site *orig, struct dive_site *copy);
void copy_dive_site(struct dive_site *orig, struct dive_site *copy);
void merge_dive_site(struct dive_site *a, struct dive_site *b);
unsigned int get_distance(const location_t *loc1, const location_t *loc2);
struct dive_site *find_or_create_dive_site_with_name(const char *name, timestamp_t divetime, struct dive_site_table *ds_table);
void merge_dive_sites(struct dive_site *ref, struct dive_site *dive_sites[], int count);
void purge_empty_dive_sites(struct dive_site_table *ds_table);
void clear_dive_site_table(struct dive_site_table *ds_table);

#ifdef __cplusplus
}
QString constructLocationTags(struct taxonomy_data *taxonomy, bool for_maintab);

/* Make pointer-to-dive_site a "Qt metatype" so that we can pass it through QVariants */
Q_DECLARE_METATYPE(dive_site *);

#endif

#endif // DIVESITE_H
