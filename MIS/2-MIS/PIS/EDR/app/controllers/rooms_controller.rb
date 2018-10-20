class RoomsController < ApplicationController
  # GET /rooms
  # GET /rooms.json
  def index
    @offices     = OfficeDecorator.decorate_collection(Office.all_active)
    @schoolrooms = SchoolroomDecorator.decorate_collection(Schoolroom.all_active)
  end
end
