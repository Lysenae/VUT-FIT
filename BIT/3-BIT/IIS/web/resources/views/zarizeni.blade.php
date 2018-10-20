@extends('layouts.app')

@section('content')

<div class="container">
    <div class="">
	<div class="panel panel-default">
	    <div class="panel-heading">
		Seznam zaměstnanců
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')
		<div class="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th>#</th>
			<th>Umístěno</th>
			<th>Zodpovídá</th>
			<th>Značka</th>
			<th>Model</th>
			<th>Cena</th>
			<th></th>
		    </thead>
		    <tbody>
		    @foreach ($zarizeni as $row)
		    <tr>
			<td class="table-text">
			    {{ $row->zarID }}
			</td>
			<td class="table-text">
			    {{ $row->roomID }}
			</td>
			<td class="table-text">
			    {{ $row->prijmeni }} {{ $row->jmeno }}
			</td>
			<td class="table-text">
			    {{ $row->znacka }}
			</td>
			<td class="table-text">
			    {{ $row->model }}
			</td>
			<td class="table-text">
			@if ($row->cena > 0)
				{{ $row->cena }}
			@endif
			</td>
			<td class="table-text">
			    <a href="/zarizeni/{{ $row->zarID }}/show">upravit</a>
			</td>
		    </tr>
		    @endforeach
		    </tbody>
		</table>
		</div>
		<form action="/zarizeni" method="POST" class="form-horizontal">
		    {{ csrf_field() }}

		    <!-- Add employee Button -->
		    <div class="form-group">
			<div class="col-sm-offset-0 col-sm-6">
			    <button type="submit" class="btn btn-default">
				<i class="fa fa-plus"></i> Přidat zařízení</button>
			</div>
		    </div>
		</form>
	    </div>
	</div>
</div>

    <!-- TODO: Current employees -->
@endsection
