module FailuresHelper
  def technicians_dropdown(list, default)
    options_from_collection_for_select(list, 'id', 'fullname_with_bday', default)
  end

  def announcers_dropdown(list, default)
    options_from_collection_for_select(list, 'id', 'fullname_with_bday', default)
  end

  def devices_dropdown(list, default)
    options_from_collection_for_select(list, 'id', 'fullname', default)
  end
end
