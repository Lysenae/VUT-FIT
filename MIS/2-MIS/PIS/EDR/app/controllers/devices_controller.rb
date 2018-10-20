class DevicesController < ApplicationController
  before_action :authorize
  before_action :set_device, only: [:show, :edit, :update, :destroy]

  DEVICE_PROPERTIES = {
    brand:         nil,
    model:         nil,
    price:         0.0,
    specification: nil,
    status:        nil
  }

  # GET /devices
  # GET /devices.json
  def index
    @devices = Device.all_active
  end

  # GET /devices/1
  # GET /devices/1.json
  def show
    access(:read)
  end

  # GET /devices/new
  def new
    access(:create)
    @device    = Device.new
    @employees = Employee.all_active
    @rooms     = Room.all_active
  end

  # GET /devices/1/edit
  def edit
    access(:update)
    @device    = Device.find(params[:id])
    @employees = Employee.all_active
    @rooms     = [@device.room]
  end

  # POST /devices
  # POST /devices.json
  def create
    access(:create)
    checked_params             = check_params(DEVICE_PROPERTIES)
    checked_params["room"]     = Room.find(device_params[:room_id])
    checked_params["employee"] = Employee.find(device_params[:employee_id])
    @device = Device.new(checked_params)
    respond_to do |format|
      if @device.save
        format.html { redirect_to @device}
        format.json { render :show, status: :created, location: @device }
      else
        @employees = Employee.all
        @rooms = Room.all
        format.html { render :new }
        format.json { render json: @device.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /devices/1
  # PATCH/PUT /devices/1.json
  def update
    access(:update)
    checked_params             = check_params(DEVICE_PROPERTIES)
    checked_params["room"]     = Room.find(device_params[:room_id])
    checked_params["employee"] = Employee.find(device_params[:employee_id])
    respond_to do |format|
      if @device.update(checked_params)
        format.html { redirect_to @device}
        format.json { render :show, status: :ok, location: @device }
      else
        @employees = Employee.all
        @rooms     = Room.all_active
        format.html { render :edit }
        format.json { render json: @device.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /devices/1
  # DELETE /devices/1.json
  def destroy
    access(:destroy)
    @device.update_attribute(:active, false)
    respond_to do |format|
      format.html { redirect_to devices_url }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_device
      @device = Device.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def device_params
      params.fetch(:device, {})
    end

    def check_params(params)
      checked = {}
      params.each do |k, v|
        checked[k] = device_params[k].blank? ? v : device_params[k]
      end
      checked
    end

    def access(action)
      authorize!(action, Device.new)
    end
end
