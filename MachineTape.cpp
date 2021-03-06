#include "MachineTape.h"



MachineTape::MachineTape()
{
   this->m_iHeaderPos = 0;
   this->m_szTape = std::string(20, '_');
}


MachineTape::~MachineTape()
{
}

void MachineTape::setState(std::string szInput)
{
   if ((int)(szInput.size() + m_iHeaderPos) <= 20)
   {
      m_szTape = std::string(m_iHeaderPos, '_') + szInput + std::string(20 - szInput.size() - m_iHeaderPos, '_');
   }
   else
   {
      if (szInput.size() < 10)
      {
         m_szTape = std::string(m_iHeaderPos, '_') + szInput + std::string(10 - szInput.size(), '_');
      }
      else
      {
         m_szTape = std::string(m_iHeaderPos, '_') + szInput;
      }

   }

}

void MachineTape::setState(DString szInput)
{
   m_szTape = std::string(m_iHeaderPos, '_');
   if ((int)(szInput.size() + m_iHeaderPos) <= 20)
   {

      m_szTape = m_szTape + szInput + std::string(20 - szInput.size() - m_iHeaderPos, '_');
   }
   else
   {
      if (szInput.size() < 10)
      {
         m_szTape = m_szTape + szInput + std::string(10 - szInput.size(), '_');
      }
      else
      {
         m_szTape = m_szTape + szInput;
      }

   }

}

void MachineTape::setHeadPosition(int iHeadPos)
{
   if (iHeadPos > m_iHeaderPos)
   {
      while (m_iHeaderPos != iHeadPos)
      {
         performTransform(RIGHT, DChar("_", 0));
      }
   }
   else
   {
      m_iHeaderPos = iHeadPos;
   }

}

std::vector<std::string> MachineTape::getDrawState(bool bFixed = true) const
{
   // Calculate the adjusted head pos
   int apparentHeaderPos = 0;


   std::vector<std::string> szRetVal;
   if ((bFixed) &&
      (m_iHeaderPos >= 10))
   {
      for (int i = m_iHeaderPos - 9; i <= m_iHeaderPos; i++)
      {
         apparentHeaderPos += m_szTape[i].getString().size();
      }
      DString DSub = m_szTape.substr(m_iHeaderPos - 10, 20);
      szRetVal.push_back(DSub.getString());
      szRetVal.push_back(std::string(apparentHeaderPos, ' ') + "^");

   }
   else
   {
      for (int i = m_iHeaderPos - 9; i <= m_iHeaderPos; i++)
      {
         apparentHeaderPos += m_szTape[i].getString().size();
      }
      DString DSub = m_szTape.substr(0, 20);
      szRetVal.push_back(DSub.getString());
      szRetVal.push_back(std::string(apparentHeaderPos, ' ') + "^");
      
   }

   return szRetVal;
}



const DChar& MachineTape::getState() const
{
   return m_szTape[m_iHeaderPos];
}

bool MachineTape::performTransform(TRANSITION headMoveMent, DChar write)
{
   // Return whether the transformation was successful
   bool bRetVal;

   if ((m_iHeaderPos + headMoveMent) > (m_szTape.size() - 10))
   {
      m_szTape = m_szTape + "_";
   }

   if ((m_iHeaderPos + headMoveMent) >= 0)
   {
      writeChar(write);
      m_iHeaderPos += headMoveMent;

      bRetVal = true;
   }
   else
   {
      bRetVal = false;
   }

   return bRetVal;
}

bool MachineTape::writeChar(DChar newState)
{
   //delete &m_szTape[m_iHeaderPos];
   m_szTape.set(m_iHeaderPos, newState);
   return true;
}
