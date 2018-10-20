class SchoolroomsController < ApplicationController
  before_action :authorize
  before_action(:set_schoolroom, only: [:show, :edit, :update, :destroy])

  SCHOOLROOM_PROPERTIES = {
    seats:  0,
    rows:   0,
    blocks: 0
  }

  ROOM_PROPERTIES = {
    label:    nil,
    location: nil,
    width:    0.0,
    length:   0.0,
    height:   0.0,
  }

  # GET /schoolrooms
  # GET /schoolrooms.json
  def index
    redirect_to(rooms_path)
  end

  # GET /schoolrooms/1
  # GET /schoolrooms/1.json
  def show
    @schoolroom.decorate
  end

  # GET /schoolrooms/new
  def new
    @schoolroom = Schoolroom.new.decorate
    access(:read)
  end

  # GET /schoolrooms/1/edit
  def edit
    @schoolroom = Schoolroom.find(params[:id]).decorate
    access(:update)
  end

  # POST /schoolrooms
  # POST /schoolrooms.json
  def create
    access(:create)
    params_schoolroom         = check_schoolroom_params
    params_room               = check_room_params
    params_room['department'] = Department.find(schoolroom_params['department'])
    @room                     = Room.create(params_room)
    params_schoolroom['room'] = @room
    @schoolroom               = Schoolroom.new(params_schoolroom)
    respond_to do |format|
      if @schoolroom.save
        format.html { redirect_to(@schoolroom) }
        format.json { render(:show, status: :created, location: @schoolroom) }
      else
        format.html { render(:new) }
        format.json { render(json: @schoolroom.errors, status: :unprocessable_entity) }
      end
    end
  end

  # PATCH/PUT /schoolrooms/1
  # PATCH/PUT /schoolrooms/1.json
  def update
    access(:update)
    params_schoolroom         = check_schoolroom_params
    params_room               = check_room_params
    params_room['department'] = Department.find(schoolroom_params['department'])
    @schoolroom.room.update(params_room)
    params_schoolroom['room'] = @schoolroom.room
    respond_to do |format|
      if @schoolroom.update(params_schoolroom)
        format.html { redirect_to(@schoolroom) }
        format.json { render(:show, status: :ok, location: @schoolroom) }
      else
        format.html { render(:edit) }
        format.json { render(json: @schoolroom.errors, status: :unprocessable_entity) }
      end
    end
  end

  # DELETE /schoolrooms/1
  # DELETE /schoolrooms/1.json
  def destroy
    access(:destroy)
    @schoolroom.room.update_attribute(
      :label, "#{@schoolroom.room.label}-deleted-#{DateTime.now.strftime('%Q')}"
    )
    @schoolroom.room.update_attribute(:active, false)
    respond_to do |format|
      format.html { redirect_to(rooms_url) }
      format.json { head(:no_content) }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_schoolroom
      @schoolroom = Schoolroom.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def schoolroom_params
      params.fetch(:schoolroom, {})
    end

    def check_schoolroom_params
      checked = {}
      SCHOOLROOM_PROPERTIES.each do |k, v|
        checked[k] = schoolroom_params[k].blank? ? v : schoolroom_params[k]
      end
      checked
    end

    def check_room_params
      checked = {}
      ROOM_PROPERTIES.each do |k, v|
        checked[k] = schoolroom_params[k].blank? ? v : schoolroom_params[k]
      end
      checked
    end

    def access(action)
      authorize!(action, Schoolroom.new)
    end
end
