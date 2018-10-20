<?php

/*
|--------------------------------------------------------------------------
| Application Routes
|--------------------------------------------------------------------------
|
| Here is where you can register all of the routes for an application.
| It's a breeze. Simply tell Laravel the URIs it should respond to
| and give it the controller to call when that URI is dataed.
|
*/
use App\Zamestnanci;
use App\Oddeleni;
use App\Mistnosti;
use App\Zarizeni;
use App\Poruchy;
use App\Presuny;
use Illuminate\Http\Request;
use App\Http\Controllers\Controller;

Route::get('/', function(){
	return Redirect::to('login');
});
// Authentication
Route::get('/login', 'Auth\AuthController@getLogin');
Route::post('/login', 'Auth\AuthController@postLogin');
Route::get('/logout', 'Auth\AuthController@getLogout');



// section ZAMESTNANCI

Route::get('/zamestnanci', ['middleware' => ['auth', 'roles'], function(){
	$active = 'zamestnanci';
    $employees = DB::select("SELECT z.*, p.popis AS popis 
				FROM iis_zamestnanec AS z, iis_prava AS p 
				WHERE z.aktivni=1 AND z.prava = p.pravaID 
				ORDER BY z.prijmeni,z.jmeno");
	return View::make('zamestnanci',compact('active', 'employees'));
},'roles' => [5, 3]]);

Route::post('/zamestnanci', ['middleware' => ['auth', 'roles'], function(){

    return Redirect::to('/add_user');
},'roles' => [5, 3]]);

Route::get('/add_user', ['middleware' => ['auth', 'roles'], function(){
$active = 'zamestnanci';
    $pracoviste = DB::select("SELECT m.roomID 
				FROM iis_mistnost AS m, iis_kancelar AS k 
				WHERE m.roomID = k.roomID AND m.smazan = 0 
				ORDER BY m.roomID");
    $prava = DB::select("SELECT pravaID, popis FROM iis_prava ORDER BY pravaID");
    return View::make('add_user', compact('active', 'pracoviste', 'prava'));
},'roles' => [5, 3]]);

Route::post('/add_user', ['middleware' => ['auth', 'roles'], function (Request $request){
	$data = $request->all();
	$validator = Zamestnanci::validator($data,'create');
	if ($validator->fails())
	    return redirect()->back()->withErrors($validator)
	            ->withInput(Input::except('Heslo', 'heslo_confirm'));
	else{
	    $zamestnanec = Zamestnanci::createZam($data);
	    return Redirect::to('zamestnanci');
	}
},'roles' => [5, 3]]);

Route::group(['prefix' => 'zamestnanec/{id}'],  function () {
    
    Route::get('/show',['middleware' => ['auth', 'roles'],function($id){
$active = 'zamestnanci';
	$pracoviste = DB::select("SELECT m.roomID 
	    FROM iis_mistnost AS m, iis_kancelar AS k 
	    WHERE m.roomID = k.roomID AND smazan=0
	    ORDER BY m.roomID");
	$prava = DB::select("SELECT pravaID, popis FROM iis_prava ORDER BY pravaID");
	$rec = DB::select("SELECT * 
				FROM iis_zamestnanec 
				WHERE zamID = $id");
	$record = json_decode(json_encode($rec[0]),true);
	if ($record['mobil'] == 0)
	    $record['mobil'] = "";
	if ($record['telefon_kancelar'] == 0)
	    $record['telefon_kancelar'] = "";
        return View::make('edit_user')->with(compact('active', 'pracoviste', 'prava', 'record'));	
    },'roles' => [5, 3]]);

    Route::post('/edit', ['middleware' => ['auth', 'roles'],function (Request $request, $id){
	$data = $request->all();
	if ($data['submit'] =="save")
	{
	    $validator = Zamestnanci::validator($data, 'update');
	    if ($validator->fails())
		return redirect()->back()->withErrors($validator)
			->withInput();
	    else{
		$zamestnanec = Zamestnanci::updateZam($data,$id);
		return Redirect::to('zamestnanci');
	    }
	}
	else {
	    Zamestnanci::deleteZam($id);	
	    return Redirect::to('zamestnanci');
	}
    },'roles' => [5, 3]]);
});

// section ODDELENI
Route::get('/oddeleni', ['middleware' => ['auth', 'roles'], function(){
    $active = 'oddeleni';
    $dep = DB::select("SELECT *  
			FROM iis_oddeleni 
			WHERE smazan=0
			ORDER BY oddID");
    return View::make('oddeleni',compact('active','dep'));
},'roles' => [5, 3]]);

Route::post('/oddeleni', ['middleware' => ['auth', 'roles'], function(){
    return Redirect::to('/add_odd');
},'roles' => [5, 3]]);

Route::get('/add_odd', ['middleware' => ['auth', 'roles'], function(){
$active = 'oddeleni';
    return View::make('add_odd',compact('active'));
},'roles' => [5, 3]]);

Route::post('/add_odd', ['middleware' => ['auth', 'roles'], function (Request $request){
	$data = $request->all();
	$validator = Oddeleni::validator($data,'create',0);
	if ($validator->fails())
	    return redirect()->back()->withErrors($validator)
	            ->withInput();
	else{
	    $zamestnanec = Oddeleni::createOdd($data);
	    return Redirect::to('oddeleni');
	}
},'roles' => [5, 3]]);

Route::group(['prefix' => 'oddeleni/{id}'], function () {
    
    Route::get('/show',['middleware' => ['auth', 'roles'],function($id){
$active = 'oddeleni';
	$rec = DB::select("SELECT * 
				FROM iis_oddeleni 
				WHERE oddID = $id");
	$record = json_decode(json_encode($rec[0]),true);
        return View::make('edit_odd')->with(compact('active', 'record'));	
    },'roles' => [5, 3]]);
    
    Route::post('/edit',['middleware' => ['auth', 'roles'], function (Request $request, $id){
	$data = $request->all();
	if ($data['submit'] =="save")
	{
	    $validator = Oddeleni::validator($data, 'update',$id);
	    if ($validator->fails())
		return redirect()->back()->withErrors($validator)
			->withInput();
	    else{
		$oddeleni = Oddeleni::updateOdd($data,$id);
		return Redirect::to('oddeleni');
	    }
	}
	else
	{	    
	    Oddeleni::deleteOdd($id);	
	    return Redirect::to('oddeleni');
	}
    },'roles' => [5, 3]]);
});



// section MISTNOSTI
Route::get('/mistnosti', ['middleware' => ['auth', 'roles'], function(){
$active = 'mistnosti';
    $offices = DB::select("SELECT * 
				FROM iis_mistnost AS m, iis_kancelar AS k, iis_oddeleni AS o 
				WHERE m.roomID = k.roomID AND m.oddeleni=o.oddID AND m.smazan=0 
				ORDER BY k.roomID");
    $classes = DB::select("SELECT * 
				FROM iis_mistnost AS m, iis_ucebna AS u, iis_oddeleni AS o
				WHERE m.roomID = u.roomID AND m.oddeleni=o.oddID AND m.smazan=0
				ORDER BY u.roomID");
    return View::make('mistnosti')->with(compact('active', 'offices','classes'));
},'roles' => [5, 2]]);

Route::post('/kancelar', ['middleware' => ['auth', 'roles'], function(){
    return Redirect::to('/kancelar');
},'roles' => [5, 2]]);

Route::get('/kancelar', ['middleware' => ['auth', 'roles'], function(){
$active = 'mistnosti';
    $dep = DB::select("SELECT *  
			    FROM iis_oddeleni
			    WHERE smazan=0
			    ORDER BY oddID");
    return View::make('add_kanc', compact('active', 'dep'));
},'roles' => [5, 2]]);

Route::post('/kancelar/add', ['middleware' => ['auth', 'roles'], function (Request $request){
$active = 'mistnosti';
	$data = $request->all();
	$validator = Mistnosti::validator($data,'create','kan', 0);
	if ($validator->fails())
	    return redirect()->back()->withErrors($validator)
	            ->withInput();
	else{
	    $mistnost = Mistnosti::createMis($data,'kan');
	    return Redirect::to('mistnosti');
	}
},'roles' => [5, 2]]);

Route::group(['prefix' => 'kancelar/{id}'], function () {

    Route::get('/show',['middleware' => ['auth', 'roles'],function($id){
$active = 'mistnosti';
	$rec = DB::select("SELECT m.*, k.*, o.* 
				FROM iis_mistnost AS m, iis_kancelar AS k, iis_oddeleni AS o
				WHERE m.roomID = '$id' AND k.roomID = m.roomID AND o.oddID = m.oddeleni");
	$dep = DB::select("SELECT *
				FROM iis_oddeleni
				WHERE smazan=0
				ORDER BY oddID");
	$record = json_decode(json_encode($rec[0]),true);
        return View::make('edit_kanc')->with(compact('active', 'record','dep'));	
    },'roles' => [5, 2]]);

    Route::post('/edit',['middleware' => ['auth', 'roles'], function (Request $request, $id){
	$data = $request->all();
	if ($data['submit'] =="save")
	{
	    $validator = Mistnosti::validator($data, 'update', 'kan', $id);
	    if ($validator->fails())
		return redirect()->back()->withErrors($validator)
			->withInput();
	    else{
		$mistnost = Mistnosti::updateMis($data,$id,'kan');
		return Redirect::to('mistnosti');
	    }
	}
	else
	{
	    Mistnosti::deleteMis($id,'kan');	
	    return Redirect::to('mistnosti');
	}
    },'roles' => [5, 2 ]]);
});


Route::post('/ucebna', ['middleware' => ['auth', 'roles'], function(){
    return Redirect::to('/ucebna');
},'roles' => [5, 2 ]]);

Route::get('/ucebna', ['middleware' => ['auth', 'roles'], function(){
$active = 'mistnosti';
    $dep = DB::select("SELECT *  
				FROM iis_oddeleni 
				WHERE smazan=0
				ORDER BY oddID");
    return View::make('add_uceb', compact('active', 'dep'));
},'roles' => [5, 2 ]]);

Route::post('/ucebna/add', ['middleware' => ['auth', 'roles'], function (Request $request){
	$data = $request->all();
	$validator = Mistnosti::validator($data,'create','uce',0);
	if ($validator->fails())
	    return redirect()->back()->withErrors($validator)
	            ->withInput();
	else{
	    $mistnost = Mistnosti::createMis($data, 'uce');
	    return Redirect::to('mistnosti');
	}
},'roles' => [5, 2 ]]);

Route::group(['prefix' => 'ucebna/{id}'], function () {

    Route::get('/show',['middleware' => ['auth', 'roles'],function($id){
$active = 'mistnosti';
	$rec = DB::select("SELECT m.*, u.*, o.* 
				FROM iis_mistnost AS m, iis_ucebna AS u, iis_oddeleni AS o
				WHERE m.roomID = '$id' AND u.roomID = m.roomID AND o.oddID = m.oddeleni");
	$dep = DB::select("SELECT *  
				FROM iis_oddeleni
				WHERE smazan=0
				ORDER BY oddID");
	$record = json_decode(json_encode($rec[0]),true);
        return View::make('edit_uceb')->with(compact('active', 'record','dep'));	
    },'roles' => [5, 2 ]]);

    Route::post('/edit',['middleware' => ['auth', 'roles'], function (Request $request, $id){
	$data = $request->all();
	if ($data['submit'] =="save")
	{
	    $validator = Mistnosti::validator($data, 'update','uce',$id);
	    if ($validator->fails())
		return redirect()->back()->withErrors($validator)
			->withInput();
	    else{
		$mistnost = Mistnosti::updateMis($data,$id,'uce');
		return Redirect::to('mistnosti');
	    }
	}
	else
	{
	    Mistnosti::deleteMis($id,'uce');	
	    return Redirect::to('mistnosti');
	}
    },'roles' => [5, 2 ]]);
});


// section ZARIZENI
Route::get('/zarizeni', ['middleware' => ['auth', 'roles'], function(){
$active = 'zarizeni';
    $zarizeni = DB::select("SELECT zar.*, zam.jmeno, zam.prijmeni, m.roomID  
				FROM iis_zarizeni AS zar, iis_zamestnanec AS zam, iis_mistnost AS m 
				WHERE zar.zodpovida = zam.zamID AND zar.umisteno = m.roomID AND zam.smazan = 0 
				ORDER BY zar.zarID");
    return View::make('zarizeni')->with(compact('active', 'zarizeni'));
},'roles' => [5, 2 ]]);

Route::post('/zarizeni', ['middleware' => ['auth', 'roles'], function(){
    return Redirect::to('/add_vt');
},'roles' => [5, 2 ]]);

Route::get('/add_vt', ['middleware' => ['auth', 'roles'], function(){
$active = 'zarizeni';
    $umisteno = DB::select("SELECT m.roomID 
				FROM iis_mistnost AS m, iis_kancelar AS k 
				WHERE m.roomID = k.roomID 
				ORDER BY m.roomID");
    $zodpovida = DB::select("SELECT * 
				FROM iis_zamestnanec 
				WHERE smazan=0
				ORDER BY prijmeni");
    return View::make('add_vt', compact('active', 'umisteno', 'zodpovida'));
},'roles' => [5, 2 ]]);

Route::post('/add_vt', ['middleware' => ['auth', 'roles'], function (Request $request){
	$data = $request->all();
	$validator = Zarizeni::validator($data,'create');
	if ($validator->fails())
	    return redirect()->back()->withErrors($validator)
	            ->withInput();
	else{
	    $zarizeni = Zarizeni::createZar($data);
	    return Redirect::to('zarizeni');
	}
},'roles' => [5, 2 ]]);

Route::group(['prefix' => 'zarizeni/{id}'], function () {
    
    Route::get('/show',['middleware' => ['auth', 'roles'],function($id){
$active = 'zarizeni';
	$umisteno = DB::select("SELECT roomID 
				    FROM iis_mistnost 
				    WHERE smazan=0
				    ORDER BY roomID");
	$zodpovida = DB::select("SELECT * 
				    FROM iis_zamestnanec 
				    WHERE smazan=0
				    ORDER BY prijmeni");
	$rec = DB::select("SELECT * 
				FROM iis_zarizeni 
				WHERE zarID = $id");
	$record = json_decode(json_encode($rec[0]),true);
	if ($record['cena'] == 0)
	    $record['cena'] = "";
        return View::make('edit_vt')->with(compact('active', 'umisteno', 'zodpovida', 'record'));	
    },'roles' => [5, 2 ]]);

    Route::post('/edit',['middleware' => ['auth', 'roles'], function (Request $request, $id){
	$data = $request->all();
	if ($data['submit'] =="save")
	{
	    $validator = Zarizeni::validator($data, 'update');
	    if ($validator->fails())
		return redirect()->back()->withErrors($validator)
			->withInput();
	    else{
		$zarizeni = Zarizeni::updateZar($data,$id);
		return Redirect::to('zarizeni');
	    }
	}
	else
	{
	    Zarizeni::deleteZar($id);	
	    return Redirect::to('zarizeni');
	}
    },'roles' => [5, 2 ]]);
});

// section PORUCHY
Route::get('/poruchy', ['middleware' => ['auth', 'roles'], function(){
$active = 'poruchy';
    $neopravene = DB::select("SELECT p.*,z.zarID, z.znacka, z.model, zam.jmeno, zam.prijmeni
				FROM iis_porucha AS p, iis_zarizeni AS z,
			       	iis_zamestnanec AS zam 
				WHERE z.zarID=p.k_oprave 
				AND zam.zamID = oznamujici AND p.technik IS NULL
				ORDER BY p.poruchaID");
    $opravene = DB::select("SELECT p.*,z.zarID, z.znacka, z.model, zam.jmeno, zam.prijmeni
				FROM iis_porucha AS p, iis_zarizeni AS z,
			       	iis_zamestnanec AS zam 
				WHERE z.zarID=p.k_oprave 
				AND zam.zamID = technik
				ORDER BY p.poruchaID");
    return View::make('poruchy')->with(compact('active', 'neopravene','opravene'));
},'roles' => [5, 4 ]]);

Route::post('/poruchy/activate', ['middleware' => ['auth', 'roles'], function(Request $request){
    $data = $request->all();
    Poruchy::activate($data);
    return Redirect::to('/poruchy');
},'roles' => [5, 4 ]]);

Route::get('/porucha', ['middleware' => 'auth', function(){
$active = 'poruchy';
    $zarizeni = DB::select("SELECT * 
				FROM iis_zarizeni 
				WHERE smazan=0
				ORDER BY zarID");
    return View::make('add_porucha', compact('active', 'zarizeni'));
}]);

Route::post('/porucha/add', ['middleware' => ['auth', 'roles'], function (Request $request){
	$data = $request->all();
	$validator = Poruchy::validator($data,'create');
	if ($validator->fails())
	    return redirect()->back()->withErrors($validator)
	            ->withInput();
	else{
	    $zarizeni = Poruchy::createPor($data);
	    return redirect()->back();
	}
},'roles' => [5, 4 ]]);


// section PRESUNY
Route::get('/presuny', ['middleware' => ['auth', 'roles'], function(){
$active = 'presuny';
    $neprovedene = DB::select("SELECT p.*, zar.*, zam.* 
				FROM iis_presun AS p, iis_zarizeni AS zar,iis_zamestnanec AS zam 
				WHERE p.provedeno IS NULL AND p.zarizeni=zar.zarID AND zam.zamID=p.zadatel
				ORDER BY p.presunID");
    $provedene = DB::select("SELECT p.*, zar.*, zam.* 
				FROM iis_presun AS p, iis_zarizeni AS zar,iis_zamestnanec AS zam 
				WHERE p.provedeno IS NOT NULL AND p.zarizeni=zar.zarID AND zam.zamID=p.zadatel
				ORDER BY p.presunID");
    return View::make('presuny')->with(compact('active', 'neprovedene', 'provedene'));
},'roles' => [5, 2 ]]);

Route::post('/presuny/activate', ['middleware' => ['auth', 'roles'], function(Request $request){
    $data = $request->all();
    Presuny::activate($data);
    return Redirect::to('/presuny');
},'roles' => [5, 2 ]]);

Route::get('/presun', ['middleware' => 'auth', function(){
$active = 'presuny';
    $umisteni = DB::select("SELECT roomID 
				FROM iis_mistnost
				WHERE smazan=0 
				ORDER BY roomID");
    $zarizeni = DB::select("SELECT * 
				FROM iis_zarizeni 
				WHERE smazan=0
				ORDER BY zarID");
    return View::make('add_presun', compact('active', 'umisteni', 'zarizeni'));
}]);

Route::post('/presun/add', ['middleware' => 'auth', function (Request $request){
	$data = $request->all();
	$validator = Presuny::validator($data);
	if ($validator->fails())
	    return redirect()->back()->withErrors($validator)
	            ->withInput();
	else{
	    $zarizeni = Presuny::createPre($data);
	    return redirect()->back();
	}
}]);

Route::get('/home', ['middleware' => 'auth', function (Request $request){
    $active = 'home';
    $login = Auth::user()->login;
    $records = DB::select("SELECT z.*, p.popis
			FROM iis_zamestnanec AS z, iis_prava AS p
			WHERE z.login='$login' AND p.pravaID=z.prava");
    $rec = $records[0];
    $userid = $rec->zamID;
    $neprovedene = DB::select("SELECT p.*, zar.* 
				FROM iis_presun AS p, iis_zarizeni AS zar 
				WHERE p.provedeno IS NULL AND p.zarizeni=zar.zarID AND p.zadatel=$userid
				ORDER BY p.presunID");
    $neopravene = DB::select("SELECT p.*, zar.* 
				FROM iis_porucha AS p, iis_zarizeni AS zar 
				WHERE p.technik IS NULL AND p.k_oprave=zar.zarID AND p.oznamujici=$userid
				ORDER BY p.poruchaID");
    return View::make('home',compact('active', 'rec','neprovedene','neopravene'));
}]);
