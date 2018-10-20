# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rails db:seed command (or created alongside the database with db:setup).
#
# Examples:
#
#   movies = Movie.create([{ name: 'Star Wars' }, { name: 'Lord of the Rings' }])
#   Character.create(name: 'Luke', movie: movies.first)

def random_date
  datef = Time.new(1950, 1, 1)
  datet = Time.new(2000, 1, 1)
  Time.at((datet.to_f - datef.to_f) * rand + datef.to_f)
end

Right.create(level: 4, name: 'Employee')
Right.create(level: 3, name: 'Material Coordinator')
Right.create(level: 2, name: 'Personnel Clerk')
Right.create(level: 1, name: 'Technician')
Right.create(level: 0, name: 'Admin')

d = Department.create(shortcut: 'ÚIFS', name: 'Ústav informačních systémů')
r = Room.create(label: 'C109', department: d, location: '1. podlazie') 
o = Office.create(room: r, electrical_sockets: 5, ethernet_sockets: 3)
e = Employee.create(name: 'Admin', surname: 'Adminovic', email: 'admin@pis.pis', birth_date: random_date, office: o)
u = User.create(login: 'admin', password: 'admin123', password_confirmation: 'admin123', right: Right.admin, employee: e)

r = Room.create(label: 'B206', department: d, location: '2. podlazie') 
o = Office.create(room: r, electrical_sockets: 8, ethernet_sockets: 2)
e = Employee.create(name: 'Material', surname: 'Coordinator', email: 'mc@pis.pis', birth_date: random_date, office: o)
u = User.create(login: 'matco', password: 'matco123', password_confirmation: 'matco123', right: Right.material_coordinator, employee: e)

r = Room.create(label: 'B101', department: d, location: '1. podlazie') 
o = Office.create(room: r, electrical_sockets: 5, ethernet_sockets: 3)
e = Employee.create(name: 'Personnel', surname: 'Clerk',email: 'pc@pis.pis', birth_date: random_date, office: o)
u = User.create(login: 'persclrk', password: 'persclrk', password_confirmation: 'persclrk', right: Right.personel_clerk, employee: e)

r = Room.create(label: 'C215', department: d, location: '2. podlazie') 
o = Office.create(room: r, electrical_sockets: 5, ethernet_sockets: 3)
e = Employee.create(name: 'Technik', surname: 'Technikowski', email: 'tech@pis.pis', birth_date: random_date, office: o)
u = User.create(login: 'tech', password: 'tech1234', password_confirmation: 'tech1234', right: Right.technician, employee: e)

r = Room.create(label: 'C119', department: d, location: '1. podlazie') 
o = Office.create(room: r, electrical_sockets: 5, ethernet_sockets: 3)
e = Employee.create(name: 'Test',surname: 'Employee', email: 'employee@pis.pis', birth_date: random_date, office: o)
u = User.create(login: 'employee', password: 'iamemployee', password_confirmation: 'iamemployee', right: Right.employee, employee: e)

dp = Department.create(name: 'Ústav inteligentních systémů', shortcut: 'ÚITS')
r1 = Room.create(label: 'C108', department: dp, location: '2. podlazie') 
o  = Office.create(room: r1, electrical_sockets: 5, ethernet_sockets: 3)
e  = Employee.create(name: 'Borek', surname: 'Stavitel', email: 'borek@hornbach.cz', birth_date: random_date, office: o)
r2 = Room.create(label: 'D105', department: dp) 
s  = Schoolroom.create(room: r2, seats: 100, rows: 5, blocks: 1)
u  = User.create(login: 'borek', password: 'password', password_confirmation: 'password', right: Right.employee, employee: e)
dv = Device.create(brand: 'First', model: 'Device D1', price: 30.5, employee: e, room: r1)
dv2 = Device.create(brand: 'Second', model: 'Device D2', price: 105, employee: e, room: r2)
t  = Transfer.create(device: dv, from: r1, to: r2, author: e, approved: e, done_at:
       Time.now, requested_at: Time.now, reason: 'aaa')
f  = Failure.create(device: dv, technician: e, announcer: e, announced_at: Time.now,
       description: 'SOmething went wrong')
