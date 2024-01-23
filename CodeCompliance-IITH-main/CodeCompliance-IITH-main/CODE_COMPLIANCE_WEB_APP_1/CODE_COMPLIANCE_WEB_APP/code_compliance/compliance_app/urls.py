from django.urls import path
from . import views
from django.urls import re_path

urlpatterns = [
    # path('admin/', admin.site.urls),
    path('raj/', views.current_datetime),
    path('run_script/', views.run_script, name='run_script'),
    path('download/', views.download_folder, name='download_folder'),
    path('uploads/', views.index),
    # path('open_local_html/', views.open_local_html, name='open_local_html'),
    # path('open_local_html/statistics.html/', views.open_local_html_statistics, name='open_local_html_statistics'),
    # re_path("r'^open_local_html/.*clang-tidy.*$'/", views.open_local_html_error, name='open_local_html_error')
    
    ]
