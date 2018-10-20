class OfficesController < ApplicationController
  before_action :authorize
  before_action(:set_office, only: [:show, :edit, :update, :destroy])

  OFFICE_PROPERTIES = {
    electrical_sockets: 0,
    ethernet_sockets:   0
  }

  ROOM_PROPERTIES = {
    label:    nil,
    location: nil,
    width:    0.0,
    length:   0.0,
    height:   0.0,
  }

  # GET /offices
  # GET /offices.json
  def index
    redirect_to(rooms_path)
  end

  # GET /offices/1
  # GET /offices/1.json
  def show
    access(:read)
  end

  # GET /offices/new
  def new
    @office = Office.new.decorate
    access(:create)
  end

  # GET /offices/1/edit
  def edit
    @office = Office.find(params[:id]).decorate
    access(:update)
  end

  # POST /offices
  # POST /offices.json
  def create
    access(:create)
    params_office             = check_params(OFFICE_PROPERTIES)
    params_room               = check_params(ROOM_PROPERTIES)
    params_room['department'] = Department.find(office_params['department'])
    @room                     = Room.create(params_room)
    params_office['room']     = @room
    @office                   = Office.new(params_office)
    respond_to do |format|
      if @office.save
        format.html { redirect_to @office}
        format.json { render :show, status: :created, location: @office }
      else
        format.html { render :new }
        format.json { render json: @office.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /offices/1
  # PATCH/PUT /offices/1.json
  def update
    access(:update)
    params_office             = check_params(OFFICE_PROPERTIES)
    params_room               = check_params(ROOM_PROPERTIES)
    params_room['department'] = Department.find(office_params['department'])
    @office.room.update(params_room)
    params_office['room'] = @office.room
    respond_to do |format|
      if @office.update(params_office)
        format.html { redirect_to(@office) }
        format.json { render(:show, status: :ok, location: @office) }
      else
        format.html { render(:edit) }
        format.json { render(json: @office.errors, status: :unprocessable_entity) }
      end
    end
  end

  # DELETE /offices/1
  # DELETE /offices/1.json
  def destroy
    access(:destroy)
    @office.room.update_attribute(
      :label, "#{@office.room.label}-deleted-#{DateTime.now.strftime('%Q')}")
    @office.room.update_attribute(:active, false)
    respond_to do |format|
      format.html { redirect_to(rooms_url) }
      format.json { head(:no_content) }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_office
      @office = Office.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def office_params
      params.fetch(:office, {})
    end

    def check_params(params)
      checked = {}
      params.each do |k, v|
        checked[k] = office_params[k].blank? ? v : office_params[k]
      end
      checked
    end

    def access(action)
      authorize!(action, Office.new)
    end
end
