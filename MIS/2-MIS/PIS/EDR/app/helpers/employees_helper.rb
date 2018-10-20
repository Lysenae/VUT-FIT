module EmployeesHelper
  def offices_dropdown(list, default)
    options_from_collection_for_select(list, 'id', 'room_label', default)
  end
end
