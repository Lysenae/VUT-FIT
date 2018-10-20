<!-- resources/views/add_user.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
<div class="col-sm-offset-3 col-sm-6">
    <div class="panel panel-default">
	<div class="panel-heading">
	    Žádost o přesun zařízení do jiné místnosti
	</div>

	<div class="panel-body">
	<!-- Display Validation Errors -->
	@include('common.errors')

<form method="POST" action="/presun/add" class="form-horizontal">
    {!! csrf_field() !!}
<div class="form-group">
    <label for="zarizeni" class="col-sm-5 control-label">
	Zařízení*
    </label>
    <div class="col-sm-7">	
	<select name="zarizeni" id="zarizeni" class="form-control" >
	    <option value="">--vyberte hodnotu--</option>
    @foreach ($zarizeni as $row)
	<option value="{{ $row->zarID }}" {{ old('zarizeni') == 
	    $row->zarID ? "selected":"" }} >
	#{{ $row->zarID }} {{ $row->znacka }} {{ $row->model }}
	</option>   
    @endforeach	
    </select>
    </div>
</div>

<div class="form-group">
    <label for="kam" class="col-sm-5 control-label">
    Nové umístění*
    </label>
    <div class="col-sm-7">	
	<select name="kam" id="kam" class="form-control">
	    <option value="">--vyberte hodnotu--</option>
    @foreach ($umisteni as $row)
	    <option value="{{ $row->roomID }}" {{ old('kam') == 
		$row->roomID ? "selected":"" }} >
	    {{ $row->roomID }}
	    </option>   
	    @endforeach
    </select>	    
    </div>
</div>

<div class="form-group">
    <label for="duvod" class="col-sm-5 control-label">
    Důvod přesunu
    </label>
    <div class="col-sm-7">	
    <textarea name="duvod" id="duvod" class="form-control"
	placeholder="Důvod přesunu">{{ old('duvod') }}</textarea>
    </div>
</div>
<div class="col-sm-offset-5 col-sm-7">
	* povinné údaje
</div>
<br><br>
<div class=" col-sm-offset-5 col-sm-7">
    <button type="submit" class="btn btn-default">
    <i class="fa fa-plus"></i>Odeslat</button>
</div>
</form>

	</div>
    </div>
</div>
</div>
@endsection
