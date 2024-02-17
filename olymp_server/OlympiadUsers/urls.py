from django.urls import path

from .views import *

urlpatterns = [
    path('signup/', signup, name='signup'), # SignUpView.as_view()
    path('access/', access, name='access'),
    path('profile/', update_profile, name='update_profile'),
    path('logout_request/', logout_request, name='custom_logout'),
    path('403', view403, name='403')
]