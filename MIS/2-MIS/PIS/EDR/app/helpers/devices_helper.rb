module DevicesHelper
  def employees_dropdown(list, default)
    options_from_collection_for_select(list, 'id', 'fullname_with_bday', default)
  end

  def rooms_dropdown(list, default)
    options_from_collection_for_select(list, 'id', 'label', default)
  end
end
