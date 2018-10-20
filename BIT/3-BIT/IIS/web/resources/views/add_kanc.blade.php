<!-- resources/views/add_user.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default">
	    <div class="panel-heading">
				Přidání kanceláře
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

<form method="POST" action="/kancelar/add" class="form-horizontal">
	{!! csrf_field() !!}

<div class="form-group">
	<label for="roomID" class="col-sm-5 control-label">
		Název místnosti*
	</label>
	<div class="col-sm-7">	
		<input type="text" name="roomID" id="roomID" class="form-control" 
		value="{{ old('roomID') }}" placeholder="Název místnosti">
	</div>
</div>

<div class="form-group">
	<label for="oddeleni" class="col-sm-5 control-label">
		Oddělení*
	</label>	
<div class="col-sm-7">	
<select name="oddeleni" id="oddeleni" class="form-control">
		<option value="">--vyberte hodnotu--</option>
		@foreach ($dep as $row)
		<option value="{{ $row->oddID }}" {{ old('nazev') == 
		    $row->oddID ? "selected":"" }} >{{ $row->nazev }}</option>   
		@endforeach	
	</select>
</div>
</div>

<div class="form-group">
	<label for="lokace" class="col-sm-5 control-label">
		Lokace kanceláře
	</label>
<div class="col-sm-7">	
	<input type="text" name="lokace" id="lokace" class="form-control" 
		value="{{ old('lokace') }}" placeholder="Lokace">
</div>
</div>

<div class="form-group">
	<label for="sirka" class="col-sm-5 control-label">
		Rozměry*
	</label>
<div class="col-sm-7">	
	   <input type="text" name="sirka" id="sirka" class="form-control" 
value="{{ old('sirka') }}" placeholder="Šířka [m]">
	   <input type="text" name="delka" id="delka" class="form-control" 
value="{{ old('delka') }}" placeholder="Délka [m]">
	   <input type="text" name="vyska" id="vyska" class="form-control" 
value="{{ old('vyska') }}" placeholder="Výška [m]">
</div>
</div>

<div class="form-group">
	<label for="el_zasuvky" class="col-sm-5 control-label">
		Elektrických zásuvek*
	</label>
	<div class="col-sm-7">	
		<input type="text" name="el_zasuvky" id="el_zasuvky" class="form-control" 
			value="{{ old('el_zasuvky') }}" placeholder="Elektrických zásuvek">
	</div>
</div>

<div class="form-group">
	<label for="eth_zasuvky" class="col-sm-5 control-label">
		Ethernetové zásuvky
	</label>
	<div class="col-sm-7">	
		<input type="text" name="eth_zasuvky" id="eth_zasuvky" class="form-control" 
			value="{{ old('eth_zasuvky') }}" placeholder="Ethernetové zásuvky">
	</div>
</div>

<div class="col-sm-offset-5 col-sm-7">
	* povinné údaje
</div>
<br><br>
<div class=" col-sm-offset-5 col-sm-7">
	<button type="submit" class="btn btn-default">
	<i class="fa fa-plus"></i>Přidat kancelář</button>
</div>
</form>


	    </div>
	</div>
    </div>
</div>
@endsection
