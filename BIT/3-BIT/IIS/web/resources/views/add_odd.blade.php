<!-- resources/views/add_odd.blade.php -->

@extends('layouts.app')
@section('content')
<div class="container">
    <div class="col-sm-offset-3 col-sm-6">
	<div class="panel panel-default	">
	    <div class="panel-heading">
		Přidání oddělení
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')

    <form method="POST" action="/add_odd" class="form-horizontal">
	{!! csrf_field() !!}
<div class="form-group">
	<label for="nazev" class="col-sm-5 control-label">
		Název*
	</label>
	<div class="col-sm-7">	
		<input type="text" name="nazev" id="nazev" class="form-control" 
		value="{{ old('nazev') }}" placeholder="Název místnosti">
	</div>
</div>
<div class="form-group">
	<label for="popis" class="col-sm-5 control-label">
		Popis
	</label>
	<div class="col-sm-7">	
	<textarea name="popis" id="popis" class="form-control"
		placeholder="Popis">{{ old('popis') }}</textarea>
	</div>
</div>


<div class="col-sm-offset-5 col-sm-7">
	* povinné údaje
</div> 
<br><br>
<div class=" col-sm-offset-5 col-sm-7">
	<button type="submit" class="btn btn-default">
	<i class="fa fa-plus"></i>Přidat oddělení</button>
</div>
</form>

	    </div>
	</div>
    </div>
</div>
@endsection
