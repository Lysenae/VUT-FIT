class ApplicationController < ActionController::Base
  protect_from_forgery with: :exception
  auto_session_timeout 1.hour

  rescue_from "AccessGranted::AccessDenied" do |exception|
    redirect_to(root_path, alert: "You don't have permission to access this page.")
  end

  private

  def current_user
    @current_user ||= User.find(session[:user_id]) if session[:user_id]
  end
  helper_method :current_user

  def authorize
    redirect_to(login_url, alert: 'Not logged in') if current_user.nil?
    @current_policy ||= ::AccessPolicy.new(current_user)
  end
end
