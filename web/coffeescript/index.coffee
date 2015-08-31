$ ->
  window.ranker = "bm25"
  set_ranker_callbacks()
  $(".input-group").keypress (k) ->
    if k.which == 13 # enter key pressed
      $("#search_button").click()
      return false;
  $("#search_button").click ->
    query = $("#query_text").val()
    if query.length != 0
      $("#search_results_list").empty()
      $.ajax "/search-api",
        type: "POST"
        dataType: "json"
        data: JSON.stringify
          jsonrpc: "2.0"
          method: "search"
          params:
            query_text: query
            ranker_method: window.ranker
          id: 1
        success: (data, stat, xhr) ->
          print_results(data.result)
        failure: (axhr, stat, err) ->
          $("#search_results_list").append("<li>Something bad happened!</li>")

set_ranker_callbacks = () ->
  $("#bm25").click ->
    window.ranker = "bm25"
    $("#search_concept").text("Okapi BM25")
  $("#pivoted-length").click ->
    window.ranker = "pivoted-length"
    $("#search_concept").text("Pivoted Length")
  $("#dirichlet-prior").click ->
    window.ranker = "dirichlet-prior"
    $("#search_concept").text("Dirichlet Prior")
  $("#jelinek-mercer").click ->
    window.ranker = "jelinek-mercer"
    $("#search_concept").text("Jelinek-Mercer")
  $("#absolute-discount").click ->
    window.ranker = "absolute-discount"
    $("#search_concept").text("Absolute Discount")

print_results = (result) ->
  for doc in result.results
    html = "<li><h4><a href='#'>#{doc.path}</a>"
    html += "<small class='pull-right'>#{doc.score.toFixed(4)}</small></h4></li>"
    $("#search_results_list").append(html)
