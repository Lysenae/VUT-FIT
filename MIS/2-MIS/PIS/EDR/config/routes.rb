Rails.application.routes.draw do
  root 'welcome#index'
  get 'login',  to: 'sessions#new',     as: 'login'
  get 'logout', to: 'sessions#destroy', as: 'logout'
  get 'rooms',  to: 'rooms#index'
  get 'active', to: 'session#active', as: 'active'
  get 'timeout', to: 'session#timeout', as: 'timeout'

  resources :failures
  resources :transfers
  resources :devices
  resources :users
  resources :schoolrooms
  resources :offices
  resources :employees
  resources :departments
  resources :sessions
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
  get '*path', to: 'welcome#index' # TODO: 404 Not Found
end
