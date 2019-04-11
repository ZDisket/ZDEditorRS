#pragma once

#define MI(cont,itn) auto itn = cont.begin()

#define IF_ERROR(cond,errcap) if (cond){QMessageBox(QMessageBox::Icon::Critical,"Error",errcap,QMessageBox::Ok,this).exec(); return;}

#define AIF_RETURN(cond) if (cond){return;}
#define AIF_RETURN_(cond,ret) if (cond){return ret;}






