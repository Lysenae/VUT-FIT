module ApplicationHelper
  def field_params(value = nil, required = nil, label = nil)
    params = { control_col: 'col-sm-9' }
    params = params.merge(value: value) if value
    params = params.merge(required: required) if required
    params = params.merge(label: label) if label
    params
  end

  def decimal_num_field_params(value = nil, required = nil, label = nil)
    field_params(value, required, label).merge(step: 0.1, in: 0...10000000)
  end

  def integer_num_field_params(value = nil, required = nil, label = nil)
    field_params(value, required, label).merge(step: 1, in: 0...1000)
  end

  def form_class
    { class: "form-control" }
  end
end
