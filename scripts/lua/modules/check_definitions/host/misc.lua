--
-- (C) 2019-22 - ntop.org
--

local checks = require("checks")
local host_alert_keys = require "host_alert_keys"

-- #################################################################

local misc = {
  -- Script category
  category = checks.check_categories.network,

  default_enabled = false,
  alert_id = host_alert_keys.host_alert_misc_traffic,

  default_value = {
     items = { 1048576, 0, 92 }, -- 1MB gt, 0 lt, SSH
  },

  gui = {
    i18n_title = "alerts_thresholds_config.misc_traffic",
    i18n_description = "alerts_thresholds_config.alert_misc_description",
    input_builder = "misc_traffic",
    input_title = "alerts_thresholds_config.misc_traffic",
    input_description = "alerts_thresholds_config.alert_misc_description",
  },
}

-- #################################################################

return misc
