class TransfersController < ApplicationController
  before_action :authorize
  before_action :set_transfer, only: [:show, :edit, :update, :destroy]

  # GET /transfers
  # GET /transfers.json
  def index
    @transfers = TransferDecorator.decorate_collection(Transfer.all_active)
  end

  # GET /transfers/1
  # GET /transfers/1.json
  def show
    access(:read)
    @transfer = @transfer.decorate
  end

  # GET /transfers/new
  def new
    access(:create)
    @transfer  = Transfer.new.decorate
    @devices   = Device.all_not_being_transfered
    @employees = Employee.all_active
    @rooms     = Room.all_active
  end

  # GET /transfers/1/edit
  def edit
    access(:update)
    @transfer  = Transfer.find(params[:id])
    @from      = Room.find(@transfer.from_id).label
    @to        = Room.find(@transfer.to_id).label
    @devices   = Device.all_not_being_transfered
    @employees = Employee.all_active
    @rooms     = Room.all_active
    @device    = Device.find(@transfer.device.id)
    @transfer  = @transfer.decorate
  end

  # POST /transfers
  # POST /transfers.json
  def create
    access(:create)
    checked_params = Hash.new
    checked_params["device"]       = Device.find(transfer_params[:device_id])
    checked_params["author"]       = Employee.find(transfer_params[:author_id])
    checked_params["from"]         = checked_params["device"].room
    checked_params["to"]           = Room.find(transfer_params[:to_id])
    checked_params["reason"]       = transfer_params[:reason]
    checked_params["requested_at"] = transfer_params[:requested_at] || DateTime.now
    checked_params["done_at"]      = transfer_params[:done_at]
    @transfer = Transfer.new(checked_params)
    respond_to do |format|
      if @transfer.save
        format.html { redirect_to @transfer}
        format.json { render :show, status: :created, location: @transfer }
      else
        @devices = Device.all_not_being_transfered
        @employees = Employee.all
        @rooms = Room.all
        format.html { render :new }
        format.json { render json: @transfer.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /transfers/1
  # PATCH/PUT /transfers/1.json
  def update
    access(:update)
    checked_params = Hash.new
    checked_params["device"]       = Transfer.find(params[:id]).device
    checked_params["author"]       = Employee.find(transfer_params[:author_id])
    checked_params["approved"]     = Employee.find(transfer_params[:approved_id])
    checked_params["from"]         = Transfer.find(params[:id]).from
    checked_params["to"]           = Room.find(transfer_params[:to_id])
    checked_params["reason"]       = transfer_params[:reason]
    checked_params["requested_at"] = transfer_params[:requested_at]
    checked_params["done_at"]      = transfer_params[:done_at]

    respond_to do |format|
      if @transfer.update(checked_params)
        if checked_params["done_at"] != nil
          checked_params["device"].update_attribute(:room, checked_params["to"])
        end
        format.html { redirect_to @transfer}
        format.json { render :show, status: :ok, location: @transfer }
      else
        @devices = Device.all_not_being_transfered
        @employees = Employee.all
        @rooms = Room.all
        @device = Device.find(@transfer.device.id)
        format.html { render :edit }
        format.json { render json: @transfer.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /transfers/1
  # DELETE /transfers/1.json
  def destroy
    access(:destroy)
    @transfer.update_attribute(:active, false)
    respond_to do |format|
      format.html { redirect_to transfers_url}
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_transfer
      @transfer = Transfer.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def transfer_params
      params.fetch(:transfer, {})
    end

    def access(action)
      authorize!(action, Transfer.new)
    end
end
