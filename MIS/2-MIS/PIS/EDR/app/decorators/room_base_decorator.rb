class RoomBaseDecorator < ApplicationDecorator
  delegate_all

  def department
    room && room.department && room.department.id ? room.department.id : nil
  end

  def dpt_shortcut
    room && room.department && room.department.shortcut ? room.department.shortcut : nil
  end

  def room_label
    room && room.label ? room.label : nil 
  end

  def location
    room && room.location ? room.location : nil
  end

  def width
    room && room.width ? room.width : 0
  end

  def length
    room && room.length ? room.length : 0
  end

  def height
    room && room.height ? room.height : 0
  end

  def departments_dropdown
    h.options_from_collection_for_select(departments, 'id', 'shortcut', department)
  end
end
