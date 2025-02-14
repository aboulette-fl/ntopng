<!-- (C) 2022 - ntop.org     -->
<template>
<div>  
<table ref="table_id" class="table w-100 table-striped table-hover table-bordered mt-3">
  <thead>
    <tr>
      <th v-for="item in columns_config">{{ item.columnName }}</th>
    </tr>
  </thead>
  <tbody></tbody>
</table>
</div>
</template>

<script setup>
import { ref, onMounted, onBeforeUnmount } from "vue";
import { ntopng_url_manager } from "../services/context/ntopng_globals_services";

const props = defineProps({
  table_buttons: Array,
  columns_config: Array,
  data_url: String,
  enable_search: Boolean,
  filter_buttons: {
    type: Array,
    required: false,
  },
  table_config: {
    type: Object,
    required: false,
  },
  base_url: String,
  base_params: Object, 
});

let new_params = props.base_params
const table_id = ref(null);
// let _this = getCurrentInstance().ctx;

let table = null;
onMounted(() => {
  let updated = false;
  /* Create a datatable with the buttons */
  let extend_config = {
    serverSide: false,
    destroy: true,
    searching: props.enable_search,
    order: [[0, "asc"]],
    pagingType: 'full_numbers',
    columnDefs: {},
    autoWidth: false,
    ajax: {
      method: 'get',
      url: props.data_url,
      dataSrc: 'rsp',
      data: (data, settings) => {
        if(Object.keys(data).length == 0) {
          return;
        }

        const tableApi = settings.oInstance.api();
        const orderColumnIndex = data.order[0].column;
        const orderColumnName = tableApi.column(orderColumnIndex).name() || undefined;
        
        if (data.order) {
          data.order = data.order[0].dir;
          data.sort = orderColumnName;
        }

        if (data.columns !== undefined) {
          delete data.columns;
        }

        if (data.search !== undefined) {
          data.map_search = data.search.value;
          delete data.search
        }
        
        return data;
      },
      beforeSend: function() {
        NtopUtils.showOverlays();
      },
      complete: function() {
        NtopUtils.hideOverlays();
        ntopng_events_manager.emit_custom_event(ntopng_custom_events.DATATABLE_LOADED);
      }
    },
    columns: props.columns_config,
  };

  for (const item in (props.table_config || {})) {
    extend_config[item] = props.table_config[item]
  }

  let config = DataTableUtils.getStdDatatableConfig(props.table_buttons);
  config = DataTableUtils.extendConfig(config, extend_config);
  table = $(table_id.value).DataTable(config);
  for (const filter of (props.filter_buttons || [])) {
    /* Set filters to active if available in the url */
    const curr_value = ntopng_url_manager.get_url_entry(filter.filterMenuKey)
    if(curr_value && curr_value != '') {
      let num_non_active_entries = 0
      filter.filters.forEach((i) => {
        i.currently_active = false
        num_non_active_entries += 1
        if(i.id == curr_value) {
          i.currently_active = true
          num_non_active_entries -= 1
        }
      })

      if(num_non_active_entries == filter.filters.length) {
        ntopng_url_manager.set_key_to_url(filter.filterMenuKey, '');
        updated = true
      }
    }

    new DataTableFiltersMenu({
      filterTitle: filter.filterTitle,
      tableAPI: table,
      filters: filter.filters,
      filterMenuKey: filter.filterMenuKey,
      columnIndex: filter.columnIndex,
      url: props.data_url,
      id: filter.id,
      removeAllEntry: filter.removeAllEntry,
      callbackFunction: filter.callbackFunction
    }).init();
  }

  if(updated && props.base_params) {
    const entries = ntopng_url_manager.get_url_entries()
    for(const [key, value] of (entries)) {
      new_params[key] = value
    }
    table.ajax.url(NtopUtils.buildURL(`${http_prefix}/lua/pro/enterprise/get_map.lua`, new_params))
    reload()
  }
});

const reload = () => {
  if (table == null) { return; }
  table.ajax.reload();
}

const update_url = (new_url) => {
  if (table == null) { return; }
  table.ajax.url(new_url);
}

const delete_button_handlers = (handlerId) => {
    DataTableUtils.deleteButtonHandlers(handlerId);
};

let is_destroyed = false;

const destroy_table = () => {
    table.clear();
    table.destroy(true);
    is_destroyed = true;
    props.columns_config.filter((config) => config.handlerId != null).forEach((config) => {
	delete_button_handlers(config.handlerId);
    });
}

defineExpose({ reload, delete_button_handlers, destroy_table, update_url });

onBeforeUnmount(() => {
    if (is_destroyed == true) { return; }
    destroy_table();
    // table.destroy(true);
});

</script>

<style scoped>
</style>
