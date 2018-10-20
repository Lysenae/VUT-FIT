class FailuresController < ApplicationController
  before_action :authorize
  before_action :set_failure

  FAILURE_PROPERTIES = {
    description:  nil,
    report:       nil,
    announced_at: Time.now,
    solved:       false
  }

  # GET /failures
  # GET /failures.json
  def index
    @failures = FailureDecorator.decorate_collection(Failure.all_active)
  end

  # GET /failures/1
  # GET /failures/1.json
  def show
    access(:read)
    @failure = @failure.decorate
  end

  # GET /failures/new
  def new
    access(:create)
    @failure = Failure.new.decorate
  end

  # GET /failures/1/edit
  def edit
    access(:update)
  end

  # POST /failures
  # POST /failures.json
  def create
    access(:create)
    checked_params = check_params(FAILURE_PROPERTIES)
    checked_params["device"] = Device.find(failure_params["device_id"])
    checked_params["announcer"] = Employee.find(failure_params["announcer_id"])
    @failure = Failure.new(checked_params)
    respond_to do |format|
      if @failure.save
        format.html { redirect_to @failure}
        format.json { render :show, status: :created, location: @failure }
      else
        format.html { render :new }
        format.json { render json: @failure.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /failures/1
  # PATCH/PUT /failures/1.json
  def update
    access(:update)
    checked_params = check_params(FAILURE_PROPERTIES)
    checked_params["device"] = Device.find(failure_params["device_id"])
    checked_params["announcer"] = Employee.find(failure_params["announcer_id"])
    if failure_params["technician_id"]
      checked_params["technician"] = Employee.find(failure_params["technician_id"])
    end

    respond_to do |format|
      if @failure.update(checked_params)
        format.html { redirect_to @failure}
        format.json { render :show, status: :ok, location: @failure }
      else
        format.html { render :edit }
        format.json { render json: @failure.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /failures/1
  # DELETE /failures/1.json
  def destroy
    access(:destroy)
    @failure.update_attribute(:active, false)
    respond_to do |format|
      format.html { redirect_to failures_url}
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_failure
      @failure     = Failure.find(params[:id]) if params[:id]
      @devices     = Device.all_active
      @employees   = Employee.all_active
      @technicians = Employee.all_technicians
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def failure_params
      params.fetch(:failure, {})
    end

    def check_params(params)
      checked = {}
      params.each do |k, v|
        checked[k] = failure_params[k].blank? ? v : failure_params[k]
      end
      checked
    end

    def access(action)
      authorize!(action, Failure.new)
    end
end
